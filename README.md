# Fallback sink

This is a simple custom sink made for spdlog that will allow you to create a list of sinks and fallback on each tier if the previous sink fails.

```
auto fallback = std::make_shared<spdlog::sinks::fallback_sink>();

fallback->add_sink(std::make_shared<spdlog::sinks::sqlite_sink>("database.db"));
fallback->add_sink(std::make_shared<spdlog::sinks::simple_file_sink_st>("LogFileName.log"));
fallback->add_sink(std::make_shared<spdlog::sinks::stdout_sink_st>());
fallback->add_sink(std::make_shared<spdlog::sinks::null_sink_st>());
		
auto logging = std::make_shared<spdlog::logger>("fallback_logger", fallback);
```

### Tier

spdlog provides multiple sinks:

* Console
* File logs (Multiple Sinks here)
* Sqlite (soon)
* Syslog (linux)
* and many others

Please note that this sink will only use one sink to log at a time and will only use the next available if the current sink fails.

So for example, Lets use the snippet provided above, This logger will contain 4 sinks (1 will be in use and 3 as fallbacks).
In case of an error while trying to log into the sqlite database, the logger will flush and remove the sqlite sink form the list and continue to log with the next available sink (simple_file_sink), and so forth.
