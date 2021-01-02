//
// Quantum Script Extension MD5
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "quantum-script-extension-md5-license.hpp"
#include "quantum-script-extension-md5.hpp"
#ifndef QUANTUM_SCRIPT_EXTENSION_MD5_NO_VERSION
#include "quantum-script-extension-md5-version.hpp"
#endif

#include "quantum-script-variablestring.hpp"
//
#include "quantum-script-extension-buffer-variablebuffer.hpp"

//#define QUANTUM_SCRIPT_VM_DEBUG_RUNTIME

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace MD5 {

				using namespace XYO;
				using namespace Quantum::Script;

				static TPointer<Variable> hash(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- md5-hash\n");
#endif

					return VariableString::newVariable(XYO::MD5::getHashString((arguments->index(0))->toString()));
				};

				static TPointer<Variable> hashToBuffer(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- md5-hash-to-buffer\n");
#endif
					TPointer<Variable> retV(Extension::Buffer::VariableBuffer::newVariable(16));
					((Extension::Buffer::VariableBuffer *)retV.value())->buffer.length = 16;
					XYO::MD5::hashStringToU8((arguments->index(0))->toString(), ((Extension::Buffer::VariableBuffer *)retV.value())->buffer.buffer);
					return retV;
				};

				void registerInternalExtension(Executive *executive) {
					executive->registerInternalExtension("MD5", initExecutive);
				};


				void initExecutive(Executive *executive, void *extensionId) {

					String info = "MD5\r\n";
					info << License::shortContent();

					executive->setExtensionName(extensionId, "MD5");
					executive->setExtensionInfo(extensionId, info);
#ifndef QUANTUM_SCRIPT_EXTENSION_MD5_NO_VERSION
					executive->setExtensionVersion(extensionId, Extension::MD5::Version::versionWithBuild());
#endif
					executive->setExtensionPublic(extensionId, true);

					executive->compileStringX("Script.requireExtension(\"Buffer\");");
					executive->compileStringX("var MD5={};");
					executive->setFunction2("MD5.hash(str)", hash);
					executive->setFunction2("MD5.hashToBuffer(str)", hashToBuffer);
				};

			};
		};
	};
};

#ifdef XYO_COMPILE_DYNAMIC_LIBRARY
extern "C" QUANTUM_SCRIPT_EXTENSION_MD5_EXPORT void quantumScriptExtension(Quantum::Script::Executive *executive, void *extensionId) {
	Quantum::Script::Extension::MD5::initExecutive(executive, extensionId);
};
#endif

