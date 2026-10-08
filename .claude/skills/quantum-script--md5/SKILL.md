---
name: quantum-script--md5
description: >-
  How to use the Quantum Script MD5 extension (quantum-script--md5), the
  RFC 1321 message digest loaded with Script.requireExtension("MD5"):
  MD5.hash(str) (bytes -> 32 lowercase hex digits, never fails, non-strings
  are hashed as their text: hash(255) is the digest of "255", hash(undefined)
  the digest of "undefined") and MD5.hashToBuffer(str) (same digest as a
  16 byte Buffer); hashing a Buffer (first length bytes) and files (no
  MD5.fileHash: Shell.fileGetContentsBuffer, check for undefined first),
  md5sum lists, change detection, cache keys, HTTP Content-MD5 (Base64 of
  the raw digest) and ETag; comparing with foreign hex (lowercase it);
  why MD5 is only a checksum, never security (use SHA256 / SHA512); which
  hosts have it (quantum-script, magnet; NOT fabricare build scripts); the
  C++ side (registerInternalExtension together with Buffer, initExecutive,
  quantumScriptExtension entry point, XYO::Cryptography::MD5 hash /
  hashToU8 / processU8 / processDone). Use when writing or reviewing Quantum
  Script code that computes MD5 digests or checksums, C++ code that
  includes <XYO/QuantumScript.Extension/MD5.hpp>, a fabricare.json
  depending on "quantum-script--md5", or when working inside the
  quantum-script--md5 repository.
---

# quantum-script--md5

MD5 (RFC 1321) message digest extension of Quantum Script (see the
`quantum-script` skill for the language and its differences from
JavaScript, and the `quantum-script--buffer` skill for the `Buffer` type;
their rules apply). Purpose: **a cheap 16 byte fingerprint of any data** —
file checksums (`md5sum`), change detection, cache and deduplication keys,
HTTP `ETag` / `Content-MD5`, compatibility with software that expects MD5.
It is **not** a security primitive: collisions are practical.

Full documentation: `docs/` in the quantum-script--md5 repository
(`X:\Storage\XYO\Gitea\CPP\quantum-script--md5\docs` on this machine):
README (purpose), getting-started (build, load, hosts, C++ registration),
**script-api** (digest forms, argument conversion, security, recipes),
cpp-api, reference. When in doubt read
`source/XYO/QuantumScript.Extension/MD5/Library.cpp` (~70 lines) and
`XYO::Cryptography::MD5` (`source/XYO/Cryptography/MD5.cpp`) in the
xyo-cryptography repository.

## Script API

```javascript
Script.requireExtension("MD5");             // also loads Buffer

MD5.hash("abc");                            // "900150983cd24fb0d6963f7d28e17f72"  32 lowercase hex, never fails
MD5.hash(buffer);                           // digest of the first buffer.length bytes
MD5.hashToBuffer("abc");                    // Buffer, size == length == 16, raw digest
MD5.hashToBuffer(x).toHex() == MD5.hash(x); // always true
MD5.hash("");                               // "d41d8cd98f00b204e9800998ecf8427e"
```

## Hard rules

1. **Not for security.** Never use MD5 for passwords, signatures,
   certificates, MACs or integrity of data an attacker can supply; use
   `SHA256` / `SHA512` (and a real scheme, e.g. `OpenSSL`). There is no
   HMAC here, and `MD5.hash(secret + msg)` is not a safe MAC. MD5 is fine
   for accidental corruption, cache keys, change detection, legacy formats.
2. **The argument is converted with `toString` first, and it never fails.**
   `hash(255)` hashes the text `"255"`; `hash()` / `hash(undefined)` return
   `"5e543256c480ac577d30f76f9120eb74"`, the digest of the word
   `"undefined"`. `Shell.fileGetContents*` returns `undefined` for a missing
   file, so **check `Script.isUndefined(data)` before hashing** or a missing
   file silently gets a valid-looking digest.
3. **Bytes, not characters.** `MD5.hash("ă")` hashes UTF-8 `c4 83`. Binary
   data goes in a Buffer (`Buffer.fromHex`, `setU8`,
   `Shell.fileGetContentsBuffer`); string literals cannot hold `\0`.
4. **No `MD5.fileHash`** (SHA256 / SHA512 have one). Use
   `MD5.hash(Shell.fileGetContentsBuffer(file))`; the whole file is read
   into memory, there is no streaming API in script.
5. **Output is lowercase hex.** Compare with foreign checksums after
   `text.trim().toLowerCaseASCII()`.
6. **Hex and raw are different inputs.** `MD5.hash(MD5.hash(x))` hashes 32
   ASCII characters, `MD5.hash(MD5.hashToBuffer(x))` the 16 raw bytes.
7. **`Content-MD5` is Base64 of the raw digest**:
   `Base64.encode(MD5.hashToBuffer(body))` (`"kAFQmDzST7DWlj99KOF/cg=="`
   for `"abc"`), not the hex text. `JSON.encode(buffer)` is `null`: store
   the hex string.
8. **Not available in fabricare build scripts.** fabricare is a static host
   without `MD5`; `requireExtension("MD5")` throws `Unable to open "MD5"`
   there. Use `SHA512.hash` / `SHA512.fileHash`. Available in the
   `quantum-script` interpreter (DLL), `magnet`, and hosts that register it.
9. **`||` and `&&` evaluate both operands** in Quantum Script (no
   short-circuit): `Script.isUndefined(t) || t.trim() == h` throws on
   `undefined`. Guard with a separate `if`.
10. One engine per thread: each thread requires `MD5` itself; the functions
    are stateless.

## Recipes

```javascript
function md5File(file) {                                       // digest of a file, undefined if unreadable
	var data = Shell.fileGetContentsBuffer(file);
	if (Script.isUndefined(data)) { return undefined; };
	return MD5.hash(data);
};

out += MD5.hash(Shell.fileGetContentsBuffer(name)) + " *" + name + "\n";   // md5sum line (binary mode)

var expected = line.substring(0, 32).toLowerCaseASCII();       // parse an md5sum line
var name = line.substring(34, line.length - 34);               // substring(start, LENGTH)

var key = MD5.hash(url + "\n" + JSON.encode(options));         // cache key, safe in file names
var header = "Content-MD5: " + Base64.encode(MD5.hashToBuffer(body));
var etag = "ETag: \"" + MD5.hash(body) + "\"";
```

## C++

```cpp
#include <XYO/QuantumScript.Extension/Buffer.hpp>
#include <XYO/QuantumScript.Extension/MD5.hpp>
using namespace XYO::QuantumScript;

void initExecutive(Executive *executive) {                 // host init callback
	Extension::Buffer::registerInternalExtension(executive);  // MD5 requires Buffer
	Extension::MD5::registerInternalExtension(executive);     // scripts still requireExtension("MD5")
};

// without the script engine:
XYO::Encoding::String hex = XYO::Cryptography::MD5::hash(data);   // lowercase hex
uint8_t digest[16];
XYO::Cryptography::MD5::hashToU8(data, digest);

XYO::Cryptography::MD5 md5;            // incremental: large files, streams
md5.processU8(part, partLength);       // repeat
md5.processDone();                     // once
md5.getHashHex();                      // or md5.toU8(digest); processInit() to reuse
```

- fabricare.json dependency: `"quantum-script--md5"` (`dll-or-lib`: DLL on
  dynamic platforms, static lib on `*.static` platforms). It pulls in
  `quantum-script`, `quantum-script--console`, `quantum-script--buffer`,
  `xyo-cryptography`.
- DLL entry point `extern "C" quantumScriptExtension(Executive *, void *)`
  only with `XYO_PLATFORM_COMPILE_DYNAMIC_LIBRARY` and without
  `XYO_QUANTUMSCRIPT_EXTENSION_MD5_LIBRARY`. Static hosts must register the
  extension as internal.

## Working in this repository

- Build: `fabricare make`, `fabricare test` (runs `test/test.01`, which
  registers Console, Buffer and MD5 as internal extensions and checks known
  digests, including the 56 / 64 byte padding boundaries, in
  `test/test.01.js`; run `make` first), `fabricare install` (see the
  `fabricare` skill). `quantum-script`, `quantum-script--console`,
  `quantum-script--buffer` and `xyo-cryptography` must be installed first.
  With the SDK installed, a script can also be run directly:
  `quantum-script test/test.01.js` (uses the installed DLL).
- Native functions live in `MD5/Library.cpp` as
  `static TPointer<Variable> name(VariableFunction *, Variable *this_, VariableArray *arguments)`
  and are registered in `initExecutive` with
  `executive->setFunction2("MD5.name(args)", name)`.
- New functions: update `README.md`, `docs/script-api.md`,
  `docs/reference.md`, `test/test.01.js` and this skill; keep the API in
  step with quantum-script--sha256 / --sha512 (a `fileHash` would need
  `fileHashMD5` in xyo-cryptography `Util`).
- Code style: tabs (width 8), `.clang-format`, CRLF, statements and blocks
  end with `};`, camelCase. SPDX header: MIT for `source/` and `docs/`,
  Unlicense for `test/` and `.claude/` (see `.reuse/dep5`).
