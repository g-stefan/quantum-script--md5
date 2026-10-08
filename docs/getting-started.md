# Getting started

## 1. Build and install

The extension is built with [fabricare](https://github.com/g-stefan/fabricare),
the build tool used by all XYO C++ projects. `quantum-script` (and everything
below it: `xyo-system`, `xyo-encoding`, ...), `quantum-script--console`,
`quantum-script--buffer` and `xyo-cryptography` must be installed to the SDK
first. From the repository root:

```bash
fabricare make       # build into output/
fabricare test       # build and run test/test.01 (run make first)
fabricare install    # copy output/{bin,include,lib} to ~/.fabricare/<platform>
fabricare clean      # remove output/ and temp/
```

`fabricare.json` declares two projects:

| Project | Kind | Purpose |
|---------|------|---------|
| `quantum-script--md5` | `dll-or-lib`: shared library in a dynamic build, static library in a static build | the extension |
| `test.01` | executable, category `test` | runs `test/test.01.js` with the extension registered as internal |

After `fabricare install`, `quantum-script--md5.dll` (Windows) /
`libquantum-script--md5.so` (Linux) sits in the SDK `bin` folder next to
`quantum-script.exe`, which is where `Script.requireExtension("MD5")` finds
it.

## 2. Use it from a script

```javascript
Script.requireExtension("Console");
Script.requireExtension("MD5");

Console.writeLn(MD5.hash(""));       // d41d8cd98f00b204e9800998ecf8427e
Console.writeLn(MD5.hash("abc"));    // 900150983cd24fb0d6963f7d28e17f72

var digest = MD5.hashToBuffer("abc");    // Buffer
Console.writeLn(digest.length);          // 16
Console.writeLn(digest.getU8(0));        // 144  (0x90)
Console.writeLn(digest.toHex());         // 900150983cd24fb0d6963f7d28e17f72
```

Run it with:

```bash
quantum-script hello-md5.js
```

Hash a file:

```javascript
Script.requireExtension("Console");
Script.requireExtension("Shell");
Script.requireExtension("MD5");

var data = Shell.fileGetContentsBuffer("setup.zip");
if (Script.isUndefined(data)) {
	throw "unable to read setup.zip";
};
Console.writeLn(MD5.hash(data) + "  setup.zip");   // same line as: md5sum setup.zip
```

`Script.requireExtension("MD5")` looks for an external `quantum-script--md5`
library first (the file as named, then every include path folder: next to
the interpreter, next to the script), then for an internal extension
registered by the host. Loading twice does nothing. A missing extension
throws `Unable to open "MD5"`.

Loading `MD5` also loads `Buffer`: the `Buffer` global exists afterwards
even if the script never required it.

## 3. magnet and fabricare scripts

`magnet` (through `quantum-script--magnet`) registers `MD5` as an internal
extension, so its scripts can use it without any DLL:

```javascript
Script.requireExtension("Console");
Script.requireExtension("MD5");

Console.writeLn(MD5.hash("release-1.0"));
```

`fabricare` does **not** include `MD5`: it is a static executable with a
fixed set of internal extensions (`Console`, `Buffer`, `Shell`, `File`,
`JSON`, `SHA512`, ...), and a static host cannot load extension DLLs, so
`Script.requireExtension("MD5")` fails in a build script with
`Unable to open "MD5"`. Use `SHA512.hash` / `SHA512.fileHash` there.

## 4. Register it in a C++ host

A host that embeds Quantum Script makes `MD5` available as an internal
extension by registering it in the init callback. `MD5` requires `Buffer`
when it is loaded, so register `Buffer` too (this is what
`test/test.01.cpp` does):

```cpp
#include <XYO/QuantumScript.hpp>
#include <XYO/QuantumScript.Extension/Console.hpp>
#include <XYO/QuantumScript.Extension/Buffer.hpp>
#include <XYO/QuantumScript.Extension/MD5.hpp>

using namespace XYO::QuantumScript;

void initExecutive(Executive *executive) {
	Extension::Console::registerInternalExtension(executive);
	Extension::Buffer::registerInternalExtension(executive);
	Extension::MD5::registerInternalExtension(executive);
};

int main(int cmdN, char *cmdS[]) {
	if (ExecutiveX::initExecutive(cmdN, cmdS, initExecutive)) {
		if (!ExecutiveX::executeString(
		        "Script.requireExtension(\"Console\");"
		        "Script.requireExtension(\"MD5\");"
		        "Console.writeLn(MD5.hash(\"abc\"));")) {
			printf("%s\n", (ExecutiveX::getError()).value());
			printf("%s", (ExecutiveX::getStackTrace()).value());
		};
		ExecutiveX::endProcessing();
	};
	return 0;
};
```

Registering only makes the extension *available*: scripts still call
`Script.requireExtension("MD5")`. With the DLL build of the engine an
external `quantum-script--md5.dll` found on the include path wins over the
internal one for `requireExtension`; use
`Script.requireInternalExtension("MD5")` to force the internal one.

In the host's `fabricare.json`:

```json
{
	"name": "my-host",
	"make": "exe",
	"sourcePath": "XYO/MyHost",
	"dependency": [
		"quantum-script--md5"
	]
}
```

`quantum-script--md5` depends on `quantum-script`, `quantum-script--console`,
`quantum-script--buffer` and `xyo-cryptography`; fabricare resolves them
transitively.

## 5. Static builds

The project is `dll-or-lib`: on a static platform (for example
`win64-msvc-2026.static`, which sets `XYO_PLATFORM_COMPILE_STATIC`) the same
project builds a static library. The export macros become empty and the
`quantumScriptExtension` DLL entry point is left out (it is compiled only
with `XYO_PLATFORM_COMPILE_DYNAMIC_LIBRARY`). Defining
`XYO_QUANTUMSCRIPT_EXTENSION_MD5_LIBRARY` has the same effect when the
sources are compiled into another library.

A static host must register the extension with `registerInternalExtension`
(section 4): external DLLs cannot be loaded into a host that does not use
the engine DLL.

## 6. Threads

Each thread that runs scripts has its own engine, so every thread loads the
extension itself with `Script.requireExtension("MD5")`. The functions keep
no state: they are safe to call from any thread that loaded the extension.
