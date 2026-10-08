# API reference

## Script

Available after `Script.requireExtension("MD5")` (which also loads
`Buffer`).

| Symbol | Returns | Notes |
|--------|---------|-------|
| `MD5.hash(str)` | String | RFC 1321 digest of the bytes of `toString(str)`, 32 lowercase hex digits; never fails |
| `MD5.hashToBuffer(str)` | Buffer | same digest as 16 raw bytes, `size == length == 16`; never fails |

### Edge cases

| Expression | Result |
|------------|--------|
| `MD5.hash("")` | `"d41d8cd98f00b204e9800998ecf8427e"` |
| `MD5.hash("abc")` | `"900150983cd24fb0d6963f7d28e17f72"` |
| `MD5.hash("The quick brown fox jumps over the lazy dog")` | `"9e107d9d372bb6826bd81d3542a419d6"` |
| `MD5.hash(buffer)` | digest of the first `length` bytes |
| `MD5.hash(Buffer.fromHex("610062"))` | `"70350f6027bce3713f6b76473084309b"` (zero byte included) |
| `MD5.hash(255)` | digest of the text `"255"` |
| `MD5.hash()`, `MD5.hash(undefined)` | `"5e543256c480ac577d30f76f9120eb74"` (the text `"undefined"`) |
| `MD5.hash("ABC") == MD5.hash("abc")` | `false` (case is data) |
| `MD5.hashToBuffer("abc").toHex()` | `"900150983cd24fb0d6963f7d28e17f72"` |
| `MD5.hashToBuffer("abc").getU8(0)` | `144` |
| `MD5.hash(MD5.hash("abc"))` | digest of the 32 hex characters |
| `MD5.hash(MD5.hashToBuffer("abc"))` | `"af5da9f45af7a300e3aded972f8ff687"` (digest of the 16 raw bytes) |
| `Base64.encode(MD5.hashToBuffer("abc"))` | `"kAFQmDzST7DWlj99KOF/cg=="` (`Content-MD5`) |
| `JSON.encode(MD5.hashToBuffer("abc"))` | `null` (Buffer) |
| `MD5.fileHash` | `undefined`: not provided, hash `Shell.fileGetContentsBuffer(file)` |

### Errors

| Message | Cause |
|---------|-------|
| `Unable to open "MD5"` | the extension library was not found and no internal one is registered (for example in `fabricare` scripts) |

The two functions themselves never throw.

## C++

Namespace `XYO::QuantumScript::Extension::MD5`, umbrella header
`<XYO/QuantumScript.Extension/MD5.hpp>`.

### Library (`MD5/Library.hpp`)

| Symbol | Notes |
|--------|-------|
| `void registerInternalExtension(Executive *executive)` | register `"MD5"` as an internal extension |
| `void initExecutive(Executive *executive, void *extensionId)` | extension init, run by the engine |
| `extern "C" void quantumScriptExtension(Executive *, void *)` | DLL entry point (not in static builds) |

### Metadata

| Symbol | Notes |
|--------|-------|
| `Version::version()`, `Version::build()`, `Version::versionWithBuild()`, `Version::datetime()` | from `version.json` |
| `Copyright::copyright()`, `Copyright::publisher()`, `Copyright::company()`, `Copyright::contact()` | |
| `License::license()`, `License::shortLicense()` | MIT text |

`Version`, `Copyright` and `License` exist in every XYO library: qualify them
(`Extension::MD5::Version::versionWithBuild()`).

### Build configuration

| Name | Meaning |
|------|---------|
| `quantum-script--md5` | fabricare project, `dll-or-lib`; depends on `quantum-script`, `quantum-script--console`, `quantum-script--buffer`, `xyo-cryptography` |
| `test.01` | fabricare test project, runs `test/test.01.js` |
| `XYO_QUANTUMSCRIPT_EXTENSION_MD5_EXPORT` | export / import macro |
| `XYO_QUANTUMSCRIPT_EXTENSION_MD5_INTERNAL` | defined while building the DLL (from `QUANTUM_SCRIPT__MD5_INTERNAL`) |
| `XYO_QUANTUMSCRIPT_EXTENSION_MD5_LIBRARY` | static build: empty export macro, no DLL entry point |

### Hash function (`xyo-cryptography`)

| Symbol | Notes |
|--------|-------|
| `XYO::Cryptography::MD5::hash(const String &)` | `String`, 32 lowercase hex digits |
| `XYO::Cryptography::MD5::hashToU8(const String &, uint8_t *buffer)` | 16 raw bytes |
| `MD5()`, `processInit()`, `processU8(data, length)`, `processDone()` | incremental hashing |
| `getHashHex()`, `toU8(buffer)` | result after `processDone()` |
| `copy(const MD5 &)` | copy the running state |
