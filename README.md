# Quantum Script Extension MD5

Quantum Script extension
- MD5 (RFC 1321) message digest of strings and buffers:
`MD5.hash` returns the 128-bit digest as 32 lowercase hex digits,
`MD5.hashToBuffer` as 16 raw bytes in a `Buffer`.
- For checksums and fingerprints: verifying copies and downloads
(`md5sum`), change detection, cache keys, HTTP `ETag` and `Content-MD5`.
- Not for security: MD5 collisions are practical, use `SHA256` or `SHA512`
for passwords, signatures and integrity against attackers.

```javascript
Script.requireExtension("MD5");

MD5;
MD5.hash(str);
MD5.hashToBuffer(str);
```

Built on `quantum-script`, `quantum-script--buffer` and `xyo-cryptography`, part of the XYO C++ SDK.

## Documentation

- [Overview](docs/README.md) - purpose and design
- [Getting started](docs/getting-started.md) - build, load from a script, hosts, register in a C++ host
- [Script API](docs/script-api.md) - both functions: digest forms, argument conversion, security, recipes
- [C++ API](docs/cpp-api.md) - registration, DLL entry point, `XYO::Cryptography::MD5`
- [API reference](docs/reference.md)

A Claude Code skill for this extension is in
[.claude/skills/quantum-script--md5](.claude/skills/quantum-script--md5/SKILL.md).

## License

Copyright (c) 2016-2026 Grigore Stefan
Licensed under the [MIT](LICENSE) license.
