// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "commands/non_session_commands.h"

#include <string>

#include "commands/response.h"
#include "webdriver_error.h"

#include "base/stringprintf.h"

#include "base/sys_info.h"
//#include "chrome/common/chrome_version_info.h"

namespace webdriver {

StatusCommand::StatusCommand(
    const std::vector<std::string>& path_segments,
    const base::DictionaryValue* const parameters)
    : Command(path_segments, parameters) {}

StatusCommand::~StatusCommand() {}

bool StatusCommand::DoesGet() {
    return true;
}

void StatusCommand::ExecuteGet(Response* const response) {
    //TODO: need to check what info we shoud send
    //chrome::VersionInfo version_info;

    DictionaryValue* build_info = new DictionaryValue;
    build_info->SetString("time",
                        base::StringPrintf("%s %s PST", __DATE__, __TIME__));
    //build_info->SetString("version", version_info.Version());
    //build_info->SetString("revision", version_info.LastChange());

    DictionaryValue* os_info = new DictionaryValue;
    os_info->SetString("name", base::SysInfo::OperatingSystemName());
    os_info->SetString("version", base::SysInfo::OperatingSystemVersion());
    os_info->SetString("arch", base::SysInfo::CPUArchitecture());

    DictionaryValue* status = new DictionaryValue;
    status->Set("build", build_info);
    status->Set("os", os_info);

    response->SetValue(status);
}

GlobalLogCommand::GlobalLogCommand(
    const std::vector<std::string>& path_segments,
    const base::DictionaryValue* const parameters)
    : Command(path_segments, parameters) {}

GlobalLogCommand::~GlobalLogCommand() {}

bool GlobalLogCommand::DoesGet() {
    return true;
}

void GlobalLogCommand::ExecuteGet(Response* const response) {
    //TODO: send content of global FileLog
    // in what format? JSON? Plain text?
}

}  // namespace webdriver