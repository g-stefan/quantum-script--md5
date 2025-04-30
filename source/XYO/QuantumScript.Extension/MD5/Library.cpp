// Quantum Script Extension MD5
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/QuantumScript.Extension/MD5/Library.hpp>
#include <XYO/QuantumScript.Extension/MD5/Copyright.hpp>
#include <XYO/QuantumScript.Extension/MD5/License.hpp>
#include <XYO/QuantumScript.Extension/MD5/Version.hpp>
#include <XYO/QuantumScript.Extension/Buffer.hpp>

#include <XYO/Cryptography.hpp>

namespace XYO::QuantumScript::Extension::MD5 {

	static TPointer<Variable> hash(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- md5-hash\n");
#endif

		return VariableString::newVariable(XYO::Cryptography::MD5::hash((arguments->index(0))->toString()));
	};

	static TPointer<Variable> hashToBuffer(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- md5-hash-to-buffer\n");
#endif
		TPointer<Variable> retV(Extension::Buffer::VariableBuffer::newVariable(16));
		((Extension::Buffer::VariableBuffer *)retV.value())->buffer.length = 16;
		XYO::Cryptography::MD5::hashToU8((arguments->index(0))->toString(), ((Extension::Buffer::VariableBuffer *)retV.value())->buffer.buffer);
		return retV;
	};

	void registerInternalExtension(Executive *executive) {
		executive->registerInternalExtension("MD5", initExecutive);
	};

	void initExecutive(Executive *executive, void *extensionId) {

		String info = "MD5\r\n";
		info << License::shortLicense().c_str();

		executive->setExtensionName(extensionId, "MD5");
		executive->setExtensionInfo(extensionId, info);
		executive->setExtensionVersion(extensionId, Extension::MD5::Version::versionWithBuild());
		executive->setExtensionPublic(extensionId, true);

		executive->compileStringX("Script.requireExtension(\"Buffer\");");
		executive->compileStringX("var MD5={};");
		executive->setFunction2("MD5.hash(str)", hash);
		executive->setFunction2("MD5.hashToBuffer(str)", hashToBuffer);
	};

};

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_MD5_LIBRARY
#	ifdef XYO_PLATFORM_COMPILE_DYNAMIC_LIBRARY
extern "C" XYO_QUANTUMSCRIPT_EXTENSION_MD5_EXPORT void quantumScriptExtension(XYO::QuantumScript::Executive *executive, void *extensionId) {
	XYO::QuantumScript::Extension::MD5::initExecutive(executive, extensionId);
};
#	endif
#endif
