# C++ API

For hosts that embed Quantum Script and for maintainers of the extension.
Read the `quantum-script` repository's `docs/embedding.md` and
`docs/writing-extensions.md` first: native functions, `Variable` and
`TPointer` work the same way here.

## Headers and namespace

```cpp
#include <XYO/QuantumScript.Extension/MD5.hpp>   // Library.hpp

using namespace XYO::QuantumScript;
```

Namespace: `XYO::QuantumScript::Extension::MD5`. Export macro:
`XYO_QUANTUMSCRIPT_EXTENSION_MD5_EXPORT`:

| Define | Effect |
|--------|--------|
| `XYO_QUANTUMSCRIPT_EXTENSION_MD5_INTERNAL` (or `QUANTUM_SCRIPT__MD5_INTERNAL`, set by fabricare while building the DLL) | export macro = `XYO_PLATFORM_LIBRARY_EXPORT` |
| none | export macro = `XYO_PLATFORM_LIBRARY_IMPORT` (consumers of the DLL) |
| `XYO_QUANTUMSCRIPT_EXTENSION_MD5_LIBRARY` | export macro empty, no `quantumScriptExtension` entry point (sources compiled into another library) |
| `XYO_PLATFORM_COMPILE_STATIC` (static platforms) | `XYO_PLATFORM_LIBRARY_EXPORT` / `IMPORT` are empty |

The `quantumScriptExtension` entry point is compiled only when
`XYO_PLATFORM_COMPILE_DYNAMIC_LIBRARY` is defined and
`XYO_QUANTUMSCRIPT_EXTENSION_MD5_LIBRARY` is not.

## Registering the extension

```cpp
void Extension::MD5::registerInternalExtension(Executive *executive);
void Extension::MD5::initExecutive(Executive *executive, void *extensionId);
```

- `registerInternalExtension` registers `"MD5"` as an internal extension;
  call it from the host's init callback, together with
  `Extension::Buffer::registerInternalExtension` (see
  [Getting started](getting-started.md#4-register-it-in-a-c-host)).
- `initExecutive` is the extension's init function, run by the engine when a
  script first requires `MD5` in a thread. It sets the extension name, info
  (license text), version and public flag, then:

  ```cpp
  executive->compileStringX("Script.requireExtension(\"Buffer\");");
  executive->compileStringX("var MD5={};");
  executive->setFunction2("MD5.hash(str)", hash);
  executive->setFunction2("MD5.hashToBuffer(str)", hashToBuffer);
  ```

  Do not call it directly.
- The DLL build also exports
  `extern "C" void quantumScriptExtension(Executive *, void *)`, which
  forwards to `initExecutive`; it is what `Script.requireExtension` looks up
  in `quantum-script--md5.dll`.

## The native functions

Both are `static` in `MD5/Library.cpp` and wrap `XYO::Cryptography::MD5`
from `xyo-cryptography`:

| Script function | C++ | Returns |
|-----------------|-----|---------|
| `MD5.hash(str)` | `XYO::Cryptography::MD5::hash(arguments->index(0)->toString())` | `VariableString`, 32 lowercase hex digits |
| `MD5.hashToBuffer(str)` | `Extension::Buffer::VariableBuffer::newVariable(16)`, `buffer.length = 16`, then `XYO::Cryptography::MD5::hashToU8(text, buffer.buffer)` | `VariableBuffer`, 16 raw bytes |

With `XYO_QUANTUMSCRIPT_DEBUG_RUNTIME` defined each function prints a trace
line (`- md5-hash`, `- md5-hash-to-buffer`).

## Using the hash without the script engine

C++ code that only needs MD5 should call `xyo-cryptography` directly instead
of going through a script:

```cpp
#include <XYO/Cryptography.hpp>

using namespace XYO::Cryptography;

String hex = MD5::hash("abc");     // "900150983cd24fb0d6963f7d28e17f72"

uint8_t digest[16];
MD5::hashToU8("abc", digest);      // 90 01 50 98 ...
```

For data that does not fit in memory, or arrives in pieces, use an `MD5`
object incrementally:

```cpp
MD5 md5;                                  // constructor calls processInit()
md5.processU8(part1, part1Length);        // any number of calls, any sizes
md5.processU8(part2, part2Length);
md5.processDone();                        // padding and length, once
String hex = md5.getHashHex();            // lowercase hex
uint8_t digest[16];
md5.toU8(digest);                         // raw bytes
md5.processInit();                        // reset to hash new data
```

| Member | Behavior |
|--------|----------|
| `MD5()` | starts a new digest (`processInit`) |
| `void processInit()` | reset to the RFC 1321 initial state |
| `void processU8(const uint8_t *data, size_t length)` | add bytes; splitting the input differently gives the same digest |
| `void processDone()` | append the padding and the 64-bit bit length; call once, after the last `processU8` |
| `String getHashHex()` | 32 lowercase hex digits, after `processDone` |
| `void toU8(uint8_t *buffer)` | 16 raw bytes into `buffer`, after `processDone` |
| `void copy(const MD5 &in)` | copy the running state (hash a common prefix once, then branch) |
| `static String hash(const String &)` | one call: hex digest of the bytes of the string |
| `static void hashToU8(const String &, uint8_t *buffer)` | one call: 16 raw bytes |
| `void hashBlock(uint32_t *w)` | internal: one 64 byte block |

`String` here is `XYO::Encoding::String` and can contain zero bytes: the
whole `length()` is hashed.

## Notes for maintainers

- New functions: register them in `initExecutive` with
  `executive->setFunction2("MD5.name(args)", name)`, then update
  `README.md`, `docs/script-api.md`, `docs/reference.md`, the skill in
  `.claude/skills/quantum-script--md5/` and `test/test.01.js`.
- Keep the API in step with `quantum-script--sha256` and
  `quantum-script--sha512`, which expose `hash` and `hashToBuffer` too, plus
  `fileHash(filename)` (backed by `XYO::Cryptography::Util::fileHashSHA256`
  / `fileHashSHA512`; `xyo-cryptography` has no `fileHashMD5` yet).
- The hash function lives in `xyo-cryptography`
  (`XYO/Cryptography/MD5.cpp`); `test/test.01.js` checks known digests,
  including inputs around the 56 and 64 byte padding boundaries
  (`processDone`); run it with `fabricare test`.
