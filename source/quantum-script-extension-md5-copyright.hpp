//
// Quantum Script Extension MD5
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_EXTENSION_MD5_COPYRIGHT_HPP
#define QUANTUM_SCRIPT_EXTENSION_MD5_COPYRIGHT_HPP

#define QUANTUM_SCRIPT_EXTENSION_MD5_COPYRIGHT "Copyright (c) Grigore Stefan"
#define QUANTUM_SCRIPT_EXTENSION_MD5_PUBLISHER "Grigore Stefan"
#define QUANTUM_SCRIPT_EXTENSION_MD5_COMPANY QUANTUM_SCRIPT_EXTENSION_MD5_PUBLISHER
#define QUANTUM_SCRIPT_EXTENSION_MD5_CONTACT "g_stefan@yahoo.com"
#define QUANTUM_SCRIPT_EXTENSION_MD5_FULL_COPYRIGHT QUANTUM_SCRIPT_EXTENSION_MD5_COPYRIGHT " <" QUANTUM_SCRIPT_EXTENSION_MD5_CONTACT ">"

#ifndef XYO_RC

#	ifndef QUANTUM_SCRIPT_EXTENSION_MD5__EXPORT_HPP
#		include "quantum-script-extension-md5--export.hpp"
#	endif

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace MD5 {
				namespace Copyright {
					QUANTUM_SCRIPT_EXTENSION_MD5_EXPORT const char *copyright();
					QUANTUM_SCRIPT_EXTENSION_MD5_EXPORT const char *publisher();
					QUANTUM_SCRIPT_EXTENSION_MD5_EXPORT const char *company();
					QUANTUM_SCRIPT_EXTENSION_MD5_EXPORT const char *contact();
					QUANTUM_SCRIPT_EXTENSION_MD5_EXPORT const char *fullCopyright();
				};
			};
		};
	};
};

#endif
#endif
