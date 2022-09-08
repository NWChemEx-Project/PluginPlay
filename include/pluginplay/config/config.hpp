/*
 * Copyright 2022 NWChemEx-Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

/** @file config.hpp
 *
 *  This file provides APIs for PluginPlay (and consumers of PluginPlay) to
 *  determine what compilation options have been enabled. These functions are
 *  such that you can make runtime decisions based on them. These functions are
 *  NOT suitable for compiletime decision making, i.e. template
 *  meta-programming. Internally PluginPlay defines
 *  `src/pluginplay/config/config_impl.hpp` which can be used for TMP.
 *
 *  N.B. Having this header file be auto-generated by the build-system would
 *       allow it to be used in TMP.
 *
 */

namespace pluginplay {

/** @brief Was PluginPlay built with RocksDB support?
 *
 *  @return True if the PluginPlay library distributed with this header was
 *               built with RocksDB support and false otherwise.
 *
 */
bool with_rocksdb();

} // namespace pluginplay
