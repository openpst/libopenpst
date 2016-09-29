	/**
			* @brief StreamingDloadSerialError - Generic Sahara Serial exception
			* @super std::exception
			*/
			class StreamingDloadSerialError : public std::exception
			{
				const StreamingDloadSerialError& operator=(StreamingDloadSerialError);
				std::string _what;
				uint8_t code;
			public:
				StreamingDloadSerialError(std::string message, uint8_t code = 0) : _what(message), code(code)  { }
				StreamingDloadSerialError(const StreamingDloadSerialError& second) : _what(second._what), code(second.code) {}
				virtual ~StreamingDloadSerialError() throw() {}
				virtual const char* what() const throw () {
					return _what.c_str();
				}

				uint32_t getCode()
				{
					return code;
				}

			};

			/**
			* @brief StreamingDloadSerialInvalidArgument - Exception thrown when a method has an invalid argument
			* @super StreamingDloadSerialError, std::invalid_argument
			*/
			class StreamingDloadSerialInvalidArgument : public StreamingDloadSerialError, std::invalid_argument
			{
				const StreamingDloadSerialInvalidArgument& operator=(StreamingDloadSerialInvalidArgument);
				std::string _what;
			public:
				StreamingDloadSerialInvalidArgument(std::string message) : StreamingDloadSerialError(message), invalid_argument(message), _what(message)  {}
				StreamingDloadSerialInvalidArgument(const StreamingDloadSerialInvalidArgument& second) : StreamingDloadSerialError(second), invalid_argument(second), _what(second._what) {}
				virtual ~StreamingDloadSerialInvalidArgument() throw() {}
				virtual const char* what() const throw () {
					return _what.c_str();
				}
			};