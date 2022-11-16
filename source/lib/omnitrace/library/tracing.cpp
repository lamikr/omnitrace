// MIT License
//
// Copyright (c) 2022 Advanced Micro Devices, Inc. All Rights Reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "library/tracing.hpp"
#include "library/config.hpp"
#include "library/state.hpp"
#include "library/thread_info.hpp"

namespace omnitrace
{
namespace tracing
{
perfetto::TraceConfig&
get_perfetto_config()
{
    static auto _v = ::perfetto::TraceConfig{};
    return _v;
}

std::unique_ptr<perfetto::TracingSession>&
get_perfetto_session()
{
    static auto _v = std::unique_ptr<perfetto::TracingSession>{};
    return _v;
}

std::unordered_map<hash_value_t, std::string>&
get_perfetto_track_uuids()
{
    static thread_local auto _v = std::unordered_map<hash_value_t, std::string>{};
    return _v;
}

std::vector<std::function<void()>>&
get_finalization_functions()
{
    static auto _v = std::vector<std::function<void()>>{};
    return _v;
}

tim::hash_map_ptr_t&
get_timemory_hash_ids(int64_t _tid)
{
    static auto _v = std::array<tim::hash_map_ptr_t, omnitrace::max_supported_threads>{};
    return _v.at(_tid);
}

tim::hash_alias_ptr_t&
get_timemory_hash_aliases(int64_t _tid)
{
    static auto _v =
        std::array<tim::hash_alias_ptr_t, omnitrace::max_supported_threads>{};
    return _v.at(_tid);
}

void
record_thread_start_time()
{
    static thread_local std::once_flag _once{};
    std::call_once(_once, []() {
        thread_info::set_start(comp::wall_clock::record(), get_mode() != Mode::Sampling);
    });
}
}  // namespace tracing
}  // namespace omnitrace
