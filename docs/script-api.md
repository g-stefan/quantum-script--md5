# Script API

Everything the extension defines after `Script.requireExtension("MD5")`.
`MD5` is a plain object holding two native functions; it has no constructor
and no state.

Both functions take one argument and convert it to a string with the
engine's `toString` first. A Quantum Script string is a sequence of bytes
(UTF-8 by convention, zero bytes allowed), and MD5 works on those bytes.

## The digest

MD5 (RFC 1321) processes the input in 64 byte blocks and produces a 128-bit
digest, 16 bytes, whatever the input length. The extension gives it in two
forms:

| Function | Form | Size | `"abc"` |
|----------|------|------|---------|
| `MD5.hash` | String, lowercase hex | 32 characters | `"900150983cd24fb0d6963f7d28e17f72"` |
| `MD5.hashToBuffer` | Buffer, raw bytes | 16 bytes | `90 01 50 98 3c d2 4f b0 d6 96 3f 7d 28 e1 7f 72` |

Both forms always describe the same digest:
`MD5.hashToBuffer(x).toHex() == MD5.hash(x)`.

## `MD5.hash(str)`

Returns the digest of the bytes of `str` as a `String` of 32 lowercase
hexadecimal digits, the same text `md5sum` and most other tools print. It
never fails.

```javascript
MD5.hash("");                                              // "d41d8cd98f00b204e9800998ecf8427e"
MD5.hash("abc");                                           // "900150983cd24fb0d6963f7d28e17f72"
MD5.hash("Hello");                                         // "8b1a9953c4611296a827abf8c47804d7"
MD5.hash("The quick brown fox jumps over the lazy dog");   // "9e107d9d372bb6826bd81d3542a419d6"
MD5.hash("ă");                                             // "357cd83d70c151505df42c4f3a65be78"  (UTF-8 bytes c4 83)
```

Argument conversion — anything that is not a string is hashed as its text
form, which is rarely what you want:

| Argument | Hashed bytes | Result |
|----------|--------------|--------|
| `Buffer` | its first `length` bytes | `MD5.hash(Buffer.fromHex("610062"))` → `"70350f6027bce3713f6b76473084309b"` (`a`, `0x00`, `b`) |
| number | decimal text | `MD5.hash(255) == MD5.hash("255")` |
| `true` / `null` | `"true"` / `"null"` | `MD5.hash(null) == MD5.hash("null")` |
| missing / `undefined` | `"undefined"` | `"5e543256c480ac577d30f76f9120eb74"` — no error |
| array | elements joined with `,` | `MD5.hash([1, 2]) == MD5.hash("1,2")` |

The `undefined` case matters in practice: `Shell.fileGetContents` returns
`undefined` for a missing file, and `MD5.hash(undefined)` silently returns
the digest of the word `"undefined"`. Test the input first:

```javascript
var data = Shell.fileGetContentsBuffer(file);
if (Script.isUndefined(data)) {
	throw "unable to read " + file;
};
var digest = MD5.hash(data);
```

String literals cannot hold a zero byte (`"a\0b"` ends at the `\0`); build
binary input with a `Buffer` (`Buffer.fromHex`, `setU8`, file reads).

- The digest is always lowercase. Compare it with hex from another source
  after normalizing that source: `text.trim().toLowerCaseASCII()`.
- Hashing the hex text of a digest is not the same as hashing the digest:
  `MD5.hash(MD5.hash(x))` hashes 32 ASCII characters,
  `MD5.hash(MD5.hashToBuffer(x))` hashes the 16 raw bytes. Pick one and
  document it when chaining hashes.

## `MD5.hashToBuffer(str)`

Same digest, returned as a new **`Buffer`** (from the `Buffer` extension)
holding the 16 raw bytes.

```javascript
var d = MD5.hashToBuffer("abc");
typeof(d);       // "Buffer"
d.length;        // 16
d.size;          // 16
d.getU8(0);      // 144  (0x90)
d.toHex();       // "900150983cd24fb0d6963f7d28e17f72"
```

- `size == length == 16`, always.
- Use it for binary formats and protocols, to build other encodings of the
  digest (Base64 for `Content-MD5`), or to feed binary APIs
  (`File.writeFromBuffer`, `Shell.filePutContentsBuffer`, `Socket`
  functions).
- `JSON.encode` turns a `Buffer` into `null`: store `MD5.hash(x)` or
  `Base64.encode(MD5.hashToBuffer(x))` instead.

## Security

MD5 is **broken as a cryptographic hash**: two different inputs with the
same digest (a collision) can be computed in seconds, and chosen-prefix
collisions have been used to forge certificates and signed code. It is
still a good checksum when nobody is trying to fool it.

| Use | MD5 | Use instead |
|-----|-----|-------------|
| Detect accidental corruption (copy, download, disk) | yes | |
| Cache keys, change detection, deduplication of your own data | yes | |
| Compatibility: `md5sum` files, `ETag`, `Content-MD5`, legacy formats | yes | |
| Integrity of data an attacker can modify or supply | **no** | `SHA256`, `SHA512` |
| Password storage | **no** | a salted, slow password hash |
| Signatures, certificates, message authentication | **no** | `SHA256` / `SHA512` with a proper scheme (`OpenSSL`) |

There is no keyed variant (HMAC) in this extension. Prefixing a secret
(`MD5.hash(secret + message)`) is not a safe MAC.

## Recipes

`String.prototype.replace` replaces every occurrence, and
`substring(start, length)` takes a length, not an end index.

### Digest of a file

There is no `MD5.fileHash` (unlike `SHA256.fileHash` and
`SHA512.fileHash`). Read the file, then hash it:

```javascript
Script.requireExtension("Shell");
Script.requireExtension("MD5");

function md5File(file) {
	var data = Shell.fileGetContentsBuffer(file);
	if (Script.isUndefined(data)) {
		return undefined;
	};
	return MD5.hash(data);
};
```

The whole file is loaded into memory; there is no incremental (streaming)
API in the script extension. For very large files use a C++ host with
`XYO::Cryptography::MD5::processU8` (see [C++ API](cpp-api.md)).

### Verify an `md5sum` list

`md5sum` writes `<32 hex digits>  <name>` (text mode) or
`<32 hex digits> *<name>` (binary mode), one file per line.

```javascript
Script.requireExtension("Console");
Script.requireExtension("Shell");
Script.requireExtension("MD5");

function md5Check(listFile) {
	var text = Shell.fileGetContents(listFile);
	if (Script.isUndefined(text)) {
		throw "unable to read " + listFile;
	};
	var allOk = true;
	for (var line of text.replace("\r", "").split("\n")) {
		if (line.length < 34) {
			continue;
		};
		var expected = line.substring(0, 32).toLowerCaseASCII();
		var name = line.substring(34, line.length - 34);
		var data = Shell.fileGetContentsBuffer(name);
		var ok = false;
		if (!Script.isUndefined(data)) {
			ok = (MD5.hash(data) == expected);
		};
		Console.writeLn(name + ": " + (ok ? "OK" : "FAILED"));
		allOk = allOk && ok;
	};
	return allOk;
};
```

### Write an `md5sum` list

```javascript
var out = "";
for (var name of ["setup.zip", "readme.txt"]) {
	out += MD5.hash(Shell.fileGetContentsBuffer(name)) + " *" + name + "\n";
};
Shell.filePutContents("files.md5", out);
```

### Detect changes between runs

```javascript
var digest = MD5.hash(Shell.fileGetContentsBuffer("config.json"));
var last = Shell.fileGetContents("config.json.md5");
if (Script.isUndefined(last)) {
	last = "";   // first run
};
if (last.trim() != digest) {
	// content changed: rebuild, re-upload, ...
	Shell.filePutContents("config.json.md5", digest);
};
```

Quantum Script evaluates **both** operands of `||` and `&&`, so
`Script.isUndefined(last) || last.trim() != digest` would still call
`trim` on `undefined` and throw. Test for `undefined` in its own `if`.

### Cache key for long or binary data

```javascript
var key = MD5.hash(url + "\n" + JSON.encode(options));
var cacheFile = "cache/" + key.substring(0, 2) + "/" + key + ".bin";
```

32 hex characters are safe in file names on every system, and the first
characters spread entries evenly over sub-folders.

### HTTP `Content-MD5` and `ETag`

`Content-MD5` (RFC 1864) is the **Base64** of the raw digest, not the hex
text:

```javascript
Script.requireExtension("Base64");
Script.requireExtension("MD5");

var header = "Content-MD5: " + Base64.encode(MD5.hashToBuffer(body));
// body "abc" -> "Content-MD5: kAFQmDzST7DWlj99KOF/cg=="

var etag = "ETag: \"" + MD5.hash(body) + "\"";
```

### Compare with a published checksum

```javascript
function sameDigest(a, b) {
	return a.trim().toLowerCaseASCII() == b.trim().toLowerCaseASCII();
};

sameDigest(MD5.hash(data), "900150983CD24FB0D6963F7D28E17F72 ");   // true when data is "abc"
```

## Related extensions

| Extension | Functions | Digest | Security |
|-----------|-----------|--------|----------|
| `MD5` | `hash`, `hashToBuffer` | 16 bytes, 32 hex | checksums only, collisions are practical |
| `SHA256` | `hash`, `hashToBuffer`, `fileHash` | 32 bytes, 64 hex | secure |
| `SHA512` | `hash`, `hashToBuffer`, `fileHash` | 64 bytes, 128 hex | secure; available in fabricare build scripts |
| `Base16` / `Base64` | `encode`, `decode`, `decodeToBuffer` | | encodings for the raw digest |
| `Buffer` | `b.toHex()`, `Buffer.fromHex(str)` | | raw digest bytes ↔ hex |
