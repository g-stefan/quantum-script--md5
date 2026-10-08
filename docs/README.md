# Quantum Script Extension MD5 — Documentation

`quantum-script--md5` is the **MD5 message digest extension of Quantum
Script**. Loaded with `Script.requireExtension("MD5")`, it adds an `MD5`
object to scripts with two functions:

```javascript
MD5.hash(str);           // bytes -> "900150983cd24fb0d6963f7d28e17f72" (32 lowercase hex digits)
MD5.hashToBuffer(str);   // bytes -> Buffer with the 16 raw digest bytes
```

MD5 (RFC 1321) reduces any sequence of bytes to a 128-bit (16 byte) value.
The same input always gives the same digest, and a one bit change in the
input changes the whole digest, so MD5 is a cheap fingerprint of data: file
checksums (`md5sum`), cache and deduplication keys, HTTP `ETag` and
`Content-MD5`, identifiers expected by older software and file formats.

- **Hash bytes, not characters.** The argument is converted to a string and
  its bytes are hashed: `"ă"` (UTF-8 `c4 83`) and the same character in
  another encoding give different digests. A `Buffer` argument is hashed
  byte for byte (its first `length` bytes).
- **Two output types.** `hash` returns the digest as text, lowercase hex,
  the same form `md5sum` prints. `hashToBuffer` returns the raw 16 bytes
  in a `Buffer` from the `Buffer` extension, for binary formats and for
  other encodings (`Base64.encode(MD5.hashToBuffer(x))`).
- **Never fails.** Every value converts to a string, so both functions
  always return a digest — including for `undefined`, which hashes the text
  `"undefined"`. Check inputs (for example a file that could not be read)
  before hashing.
- **Not for security.** MD5 collisions can be produced on an ordinary
  computer in seconds. Use it to detect accidental changes, never to protect
  against deliberate ones: no passwords, signatures, certificates or
  integrity checks against an attacker. Use `SHA256` or `SHA512` there.

```
scripts: quantum-script .js, magnet, embedding hosts, ...
quantum-script--md5       <-- this extension: MD5.hash / hashToBuffer
quantum-script--buffer    (the Buffer type returned by hashToBuffer, loaded automatically)
quantum-script            (Executive, Variable, Context)
xyo-cryptography          (XYO::Cryptography::MD5, the hash function)
xyo-system, xyo-encoding, xyo-multithreading, xyo-data-structures, xyo-managed-memory, xyo-platform
```

## Why it exists

| Need | What `MD5` gives |
|------|------------------|
| Check that a file was copied or downloaded intact | `MD5.hash(Shell.fileGetContentsBuffer(file))`, compare with the published `md5sum` |
| Detect that content changed (rebuild, re-upload, cache invalidation) | store the digest, compare on the next run |
| Short, fixed size key for long or binary data (cache file names, deduplication, map keys) | 32 hex characters for any input length |
| Talk to software that expects MD5: `md5sum` files, HTTP `ETag` / `Content-MD5`, legacy databases and protocols | the standard RFC 1321 digest in hex or raw bytes |
| Same API as the other hashes | `SHA256` and `SHA512` have `hash` and `hashToBuffer` too |

Digest size: **MD5 16 bytes (32 hex)**, SHA256 32 bytes (64 hex), SHA512
64 bytes (128 hex). MD5 is the shortest and the fastest, and the only one
of the three that must not be used where an attacker can choose the data.

## Concepts at a glance

| Need | Use | Notes |
|------|-----|-------|
| Load the extension | `Script.requireExtension("MD5");` | also loads `Buffer` |
| Hex digest of a string | `MD5.hash("abc")` | `"900150983cd24fb0d6963f7d28e17f72"` |
| Hex digest of a buffer | `MD5.hash(buffer)` | first `length` bytes |
| Raw digest | `MD5.hashToBuffer("abc")` | `Buffer`, `size == length == 16` |
| Hex from the raw digest | `MD5.hashToBuffer(x).toHex()` | same as `MD5.hash(x)` |
| Base64 digest (`Content-MD5`) | `Base64.encode(MD5.hashToBuffer(x))` | `"kAFQmDzST7DWlj99KOF/cg=="` for `"abc"` |
| Digest of a file | `MD5.hash(Shell.fileGetContentsBuffer(file))` | check for `undefined` first; there is no `MD5.fileHash` |
| Compare with foreign hex | `MD5.hash(x) == text.trim().toLowerCaseASCII()` | `hash` is always lowercase |
| Empty input | `MD5.hash("")` | `"d41d8cd98f00b204e9800998ecf8427e"` |

## Contents

| Document | What it covers |
|----------|----------------|
| [Getting started](getting-started.md) | Build and install, load the extension from a script, magnet and fabricare scripts, register it in a C++ host |
| [Script API](script-api.md) | Both functions: argument conversion, output format, security notes, recipes |
| [C++ API](cpp-api.md) | `registerInternalExtension`, `initExecutive`, the DLL entry point, using `XYO::Cryptography::MD5` directly from C++ |
| [API reference](reference.md) | Every script and C++ symbol on one page |

Quantum Script itself (the language, `Script.requireExtension`, embedding,
writing extensions) is documented in the `quantum-script` repository,
`docs/`; the `Buffer` type in the `quantum-script--buffer` repository,
`docs/`; the hash function in the `xyo-cryptography` repository.

## Source map

```
source/XYO/QuantumScript.Extension/MD5.hpp            umbrella header, include this from C++
source/XYO/QuantumScript.Extension/MD5.Amalgam.cpp    the whole extension in one translation unit
source/XYO/QuantumScript.Extension/MD5/
    Dependency.hpp                                    <XYO/QuantumScript.hpp>, export macro
    Library[.hpp/.cpp]                                initExecutive, registerInternalExtension,
                                                      hash / hashToBuffer
    Copyright / License / Version                     library metadata
test/test.01.cpp                                      C++ host registering Console, Buffer, MD5 as internal
test/test.01.js                                       known digests, including inputs around the 56 / 64 byte block boundaries
```

## AI assistant skill

A Claude Code skill describing how to use this extension lives in
[`.claude/skills/quantum-script--md5/`](../.claude/skills/quantum-script--md5/SKILL.md).
It is picked up automatically inside this repository; copy the folder to
`~/.claude/skills/` to have it available in the projects that use `MD5`
(Quantum Script tools, magnet scripts, other extensions).
