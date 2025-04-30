// Quantum Script Extension MD5
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_MD5_LIBRARY_HPP
#define XYO_QUANTUMSCRIPT_EXTENSION_MD5_LIBRARY_HPP

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_MD5_DEPENDENCY_HPP
#	include <XYO/QuantumScript.Extension/MD5/Dependency.hpp>
#endif

namespace XYO::QuantumScript::Extension::MD5 {

	XYO_QUANTUMSCRIPT_EXTENSION_MD5_EXPORT void initExecutive(Executive *executive, void *extensionId);
	XYO_QUANTUMSCRIPT_EXTENSION_MD5_EXPORT void registerInternalExtension(Executive *executive);

};

#endif
