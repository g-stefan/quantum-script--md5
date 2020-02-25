//
// Quantum Script Extension MD5
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_EXTENSION_MD5_HPP
#define QUANTUM_SCRIPT_EXTENSION_MD5_HPP

#ifndef QUANTUM_SCRIPT_HPP
#include "quantum-script.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_MD5__EXPORT_HPP
#include "quantum-script-extension-md5--export.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_MD5_COPYRIGHT_HPP
#include "quantum-script-extension-md5-copyright.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_MD5_LICENSE_HPP
#include "quantum-script-extension-md5-license.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_MD5_VERSION_HPP
#include "quantum-script-extension-md5-version.hpp"
#endif

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace MD5 {

				using namespace Quantum::Script;

				QUANTUM_SCRIPT_EXTENSION_MD5_EXPORT void initExecutive(Executive *executive, void *extensionId);
				QUANTUM_SCRIPT_EXTENSION_MD5_EXPORT void registerInternalExtension(Executive *executive);

			};
		};
	};
};

#endif

