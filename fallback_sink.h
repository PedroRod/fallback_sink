//
// Copyright (c) 2015 David Schury, Gabi Melman
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
//

#pragma once

#include "details/log_msg.h"
#include "details/null_mutex.h"
#include "sinks/base_sink.h"
#include "sinks/sink.h"

#include <algorithm>
#include <memory>
#include <mutex>
#include <vector>

namespace spdlog
{
	namespace sinks
	{
		class fallback_sink : public sink
		{
		public:

			~fallback_sink()
			{
				_sinks_to_remove.clear();
				_sinks.clear();
			}

			void log(const details::log_msg& msg) override
			{
				for (auto& sink : _sinks)
				{
					try
					{
						sink->log(msg);
						break;
					}
					catch (const std::exception& ex)
					{
						sink->flush();

						_sinks_to_remove.push_back(sink);
					}
				}

				if (!_sinks_to_remove.empty())
				{
					for (auto sink_to_remove : _sinks_to_remove)
					{
						remove_sink(sink_to_remove);
					}
					_sinks_to_remove.clear();
				}
			}

			void flush() override
			{
				for (auto& sink : _sinks)
					sink->flush();
			}

			void add_sink(std::shared_ptr<sink> sink)
			{
				_sinks.push_back(sink);
			}

			void remove_sink(std::shared_ptr<sink> sink)
			{
				auto pos = std::find(_sinks.begin(), _sinks.end(), sink);

				_sinks.erase(pos);
				
			}

		private:
			std::vector<std::shared_ptr<sink>> _sinks;

			std::vector<std::shared_ptr<sink>> _sinks_to_remove;
		};
	};
}
