// Quantum Script Extension MD5
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/QuantumScript.Extension/MD5/Copyright.hpp>
#include <XYO/QuantumScript.Extension/MD5/Copyright.rh>

namespace XYO::QuantumScript::Extension::MD5::Copyright {

	static const char *copyright_ = XYO_QUANTUMSCRIPT_EXTENSION_MD5_COPYRIGHT;
	static const char *publisher_ = XYO_QUANTUMSCRIPT_EXTENSION_MD5_PUBLISHER;
	static const char *company_ = XYO_QUANTUMSCRIPT_EXTENSION_MD5_COMPANY;
	static const char *contact_ = XYO_QUANTUMSCRIPT_EXTENSION_MD5_CONTACT;

	const char *copyright() {
		return copyright_;
	};

	const char *publisher() {
		return publisher_;
	};

	const char *company() {
		return company_;
	};

	const char *contact() {
		return contact_;
	};

};
