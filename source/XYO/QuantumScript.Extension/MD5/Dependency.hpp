// Quantum Script
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_MD5_DEPENDENCY_HPP
#define XYO_QUANTUMSCRIPT_EXTENSION_MD5_DEPENDENCY_HPP

#ifndef XYO_QUANTUMSCRIPT_HPP
#	include <XYO/QuantumScript.hpp>
#endif

// -- Export

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_MD5_INTERNAL
#	ifdef QUANTUM_SCRIPT__MD5_INTERNAL
#		define XYO_QUANTUMSCRIPT_EXTENSION_MD5_INTERNAL
#	endif
#endif

#ifdef XYO_QUANTUMSCRIPT_EXTENSION_MD5_INTERNAL
#	define XYO_QUANTUMSCRIPT_EXTENSION_MD5_EXPORT XYO_LIBRARY_EXPORT
#else
#	define XYO_QUANTUMSCRIPT_EXTENSION_MD5_EXPORT XYO_LIBRARY_IMPORT
#endif

#endif
