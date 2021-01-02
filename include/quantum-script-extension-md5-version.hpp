//
// Quantum Script Extension MD5
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_EXTENSION_MD5_VERSION_HPP
#define QUANTUM_SCRIPT_EXTENSION_MD5_VERSION_HPP

#define QUANTUM_SCRIPT_EXTENSION_MD5_VERSION_ABCD                 1,1,0,3
#define QUANTUM_SCRIPT_EXTENSION_MD5_VERSION_STR                 "1.1.0"
#define QUANTUM_SCRIPT_EXTENSION_MD5_VERSION_STR_BUILD           "3"
#define QUANTUM_SCRIPT_EXTENSION_MD5_VERSION_STR_DATETIME        "2020-09-21 17:40:09"

#ifndef XYO_RC

#ifndef QUANTUM_SCRIPT_EXTENSION_MD5__EXPORT_HPP
#include "quantum-script-extension-md5--export.hpp"
#endif

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace MD5 {
				namespace Version {
					QUANTUM_SCRIPT_EXTENSION_MD5_EXPORT const char *version();
					QUANTUM_SCRIPT_EXTENSION_MD5_EXPORT const char *build();
					QUANTUM_SCRIPT_EXTENSION_MD5_EXPORT const char *versionWithBuild();
					QUANTUM_SCRIPT_EXTENSION_MD5_EXPORT const char *datetime();
				};
			};
		};
	};
};

#endif
#endif

