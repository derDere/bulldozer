// Copyright 2014-2018 Matthew Krafczyk
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

// This file was created from individual ArgParse source objects

#ifndef ARGPARSE_STANDALONE_HDR
#define ARGPARSE_STANDALONE_HDR

#ifndef ARGPARSE_ArgParse_HDR
#define ARGPARSE_ArgParse_HDR

#ifdef ARGPARSE_VERSION
#error "Please only include one version of ArgParse."
#endif
#define ARGPARSE_VERSION 0.8.2

#ifndef ARGPARSE_Message_HDR
#define ARGPARSE_Message_HDR

#include <string>
#include <cstring>

namespace ArgParse {
	const char* basename(const char* filename);
}

namespace ArgParse {
	int _vscprintf (const char* format, va_list pargs);

	const std::string& GetMessage();

	extern int DebugLevel;
	extern bool Color;

	void SetMessage(const std::string& message);
	void SetMessage(const char* format, ...);

	void SetSTDOUTChannel(FILE* stdout_channel);
	void SetSTDERRChannel(FILE* stderr_channel);

	void MessageStandardPrint(const char* format, ...);
	void MessageErrorPrint(const char* format, ...);
}

#define ARGPARSE_TNRM  "\x1B[0m"
#define ARGPARSE_TRED  "\x1B[31m"
#define ARGPARSE_TGRN  "\x1B[32m"
#define ARGPARSE_TYEL  "\x1B[33m"
#define ARGPARSE_TBLU  "\x1B[34m"
#define ARGPARSE_TMAG  "\x1B[35m"
#define ARGPARSE_TCYN  "\x1B[36m"
#define ARGPARSE_TWHT  "\x1B[37m"

#define ARGPARSE_TMSG ARGPARSE_TGRN
#define ARGPARSE_TERR ARGPARSE_TRED
#define ARGPARSE_TWRN ARGPARSE_TYEL
#define ARGPARSE_TDBG ARGPARSE_TBLU

#define ArgParseMessagePrint(format, ...) ArgParse::MessageStandardPrint(format, ##__VA_ARGS__)
#define ArgParseMessageWarning(format, ...) if(ArgParse::Color) { ArgParse::MessageStandardPrint("%s%s-W (%s:%i):%s " format, ARGPARSE_TWRN, __PRETTY_FUNCTION__, ArgParse::basename(__FILE__), __LINE__, ARGPARSE_TNRM, ##__VA_ARGS__); } else { ArgParse::MessageStandardPrint("%s-W (%s:%i): " format, __PRETTY_FUNCTION__, ArgParse::basename(__FILE__), __LINE__, ##__VA_ARGS__); }
#define ArgParseMessageDebug(format, ...) if(ArgParse::Color) { ArgParse::MessageStandardPrint("%s%s-D (%s:%i):%s " format, ARGPARSE_TDBG, __PRETTY_FUNCTION__, ArgParse::basename(__FILE__), __LINE__, ARGPARSE_TNRM, ##__VA_ARGS__); } else { ArgParse::MessageStandardPrint("%s-D (%s:%i): " format, __PRETTY_FUNCTION__, ArgParse::basename(__FILE__), __LINE__, ##__VA_ARGS__); }
#define ArgParseMessageError(format, ...) if(ArgParse::Color) { ArgParse::MessageErrorPrint("%s%s-E (%s:%i):%s " format, ARGPARSE_TERR, __PRETTY_FUNCTION__, ArgParse::basename(__FILE__), __LINE__, ARGPARSE_TNRM, ##__VA_ARGS__); } else { ArgParse::MessageErrorPrint("%s-E (%s:%i): " format, __PRETTY_FUNCTION__, ArgParse::basename(__FILE__), __LINE__, ##__VA_ARGS__); }

#endif
#ifndef ARGPARSE_Argument_HDR
#define ARGPARSE_Argument_HDR

#include <vector>
#include <string>
#ifndef ARGPARSE_ArgObject_HDR
#define ARGPARSE_ArgObject_HDR

#include <string>


namespace ArgParse {
	class ArgObject {
		public:
			//Accept management
			typedef int Accept_t;
			static const Accept_t No;
			static const Accept_t WithSingleArg;
			static const Accept_t WithMultipleArg;
			static const Accept_t WithoutArg;
			static const Accept_t GroupName;
			static const char* TranslateAccept(const Accept_t i);

			//Pass management
			typedef int Pass_t;
			static const Pass_t NotAccepted;
			static const Pass_t Accepted;
			static const Pass_t Error;
			static const char* TranslatePass(const Pass_t i);

			//Mode management
			typedef int Mode_t;
			static const Mode_t Single;
			static const Mode_t Multiple;
			static const Mode_t None;
			static const char* TranslateMode(const Mode_t i);

			//Required management
			typedef bool Req_t;
			static const Req_t Required;
			static const Req_t Optional;
			static const char* TranslateReq(const Req_t i);

			//Ready types
			typedef int State_t;
			static const State_t NotReady;
			static const State_t Defined;
			static const State_t NotDefined;
			static const State_t Ready;
			static const char* TranslateState(const State_t i);

			ArgObject(const std::string& help_text, const bool required) {
				this->help_text = help_text;
				this->required = required;
			}
			virtual ~ArgObject();

			virtual bool IsConfigured() const __attribute__((warn_unused_result)) {
				return true;
			}
			virtual Accept_t AcceptsArgument(const std::string& arg __attribute__((unused))) const __attribute__((warn_unused_result)) {
				return ArgObject::No;
			}
			virtual Pass_t PassArgument(const std::string& arg __attribute__((unused)), const std::string& opt __attribute__((unused)), const bool with_opt __attribute__((unused))) __attribute__((warn_unused_result)) {
				return ArgObject::NotAccepted;
			}

			virtual size_t AmountOfData() const {
				return 0;
			}

			virtual int State(bool quiet __attribute__((unused)) = false) const __attribute__((warn_unused_result)) {
				return ArgObject::NotReady;
			}

			virtual std::string GetHelpText() const {
				return "Empty ArgObject";
			}

			virtual Mode_t GetMode() const {
				return None;
			}

			bool GetRequired() const {
				return required;
			}

			const std::string& GetHelp() const {
				return help_text;
			}

		private:
			bool required;
			std::string help_text;
	};
}

#endif

namespace ArgParse {
	class Argument : public ArgObject {
		public:
			Argument(const std::string& call_name, const std::string& help_text, const Req_t required, bool* was_defined = nullptr);
			virtual ~Argument();

			static std::vector<std::string> GetCallNames(const std::string& combined_names);

			std::string GetName(size_t i=0) const;
			size_t GetNum() {
				return call_names.size();
			}

			//ArgObject functions
			ArgObject::State_t State(bool quiet = false) const __attribute__((warn_unused_result));
			virtual std::string GetHelpText() const;
			void PrepareHelpText(std::stringstream& ss) const;
			virtual void AppendType(std::stringstream& ss) const;
			std::string GetHelpTextWithMessage(const std::string& message) const;

		protected:
			bool DoesAnArgumentMatch(size_t& position, const std::string& arg) const __attribute__((warn_unused_result));
			bool WasDefined() const {
				return *defined;
			}
			void SetDefined(bool status) {
				*defined = status;
			}

		private:
			Req_t IsRequired() const {
				return GetRequired();
			}

			std::vector<std::string> call_names;
			bool* defined;
			bool responsible_for_defined;
	};
}

#endif
#ifndef ARGPARSE_ArgGroup_HDR
#define ARGPARSE_ArgGroup_HDR

#ifndef ARGPARSE_ArgObjContainer_HDR
#define ARGPARSE_ArgObjContainer_HDR

#ifndef ARGPARSE_ArgScalar_HDR
#define ARGPARSE_ArgScalar_HDR

#include <sstream>


namespace ArgParse {
	template<class T>
	class ArgScalar : public Argument {
		public:
			ArgScalar(const std::string& call_name, const std::string& help_text, T* value, const Req_t required, bool* was_defined = nullptr);
			~ArgScalar() {}

			virtual int SetValue(const std::string& optarg) __attribute__((warn_unused_result));

			virtual ArgObject::Accept_t AcceptsArgument(const std::string& arg __attribute__((unused))) const __attribute__((warn_unused_result));
			virtual int PassArgument(const std::string& arg, const std::string& opt, const bool with_opt) __attribute__((warn_unused_result));
			virtual size_t AmountOfData() const;

			virtual std::string GetHelpText() const;

			virtual void AppendType(std::stringstream& ss) const {
				ss << "Scalar : ";
			}

			virtual ArgObject::Mode_t GetMode() const {
				return ArgObject::Single;
			}

		private:
			T* value;
	};

	template<class T>
	ArgScalar<T>::ArgScalar(const std::string& call_name, const std::string& help_text, T* value, const Req_t required, bool* was_defined) : Argument(call_name, help_text, required, was_defined) {
		this->value = value;
	}

	template<class T>
	int ArgScalar<T>::SetValue(const std::string& optarg) {
		std::stringstream ss(optarg);
		if(!(ss >> *value)) {
			ArgParseMessageError("There was an error reading values from the string stream.\n");
			return -1;
		}
		if(ss.eof()) {
			SetDefined(true);
			return 0;
		} else {
			ArgParseMessageError("String stream has some sort of error.\n");
			return -2;
		}
	}

	template<class T>
	ArgObject::Accept_t ArgScalar<T>::AcceptsArgument(const std::string& arg) const {
		if(DebugLevel > 1) {
			MessageStandardPrint("Testing the argument (%s)\n", arg.c_str());
		}
		size_t pos;
		bool result = DoesAnArgumentMatch(pos, arg);
		if(!result) {
			return ArgObject::No;
		}

		return ArgObject::WithSingleArg;
	}

	template<class T>
	int ArgScalar<T>::PassArgument(const std::string& arg, const std::string& opt, const bool with_opt) {
		size_t pos;
		bool result = DoesAnArgumentMatch(pos, arg);
		if(!result) {
			return ArgObject::NotAccepted;
		}

		if(!with_opt) {
			ArgParseMessageError("A non-boolean argument must have a value!\n");
			SetMessage("A non-boolean argument must have a value!\n");
			return ArgObject::Error;
		}

		if(SetValue(opt) < 0) {
			ArgParseMessageError("There was a problem setting the value of the option! Arg(%s) Opt(%s)\n", arg.c_str(), opt.c_str());
			return ArgObject::Error;
		}
		SetDefined(true);
		return ArgObject::Accepted;
	}

	template<class T>
	size_t ArgScalar<T>::AmountOfData() const {
		if(!WasDefined()) {
			return 0;
		}
		return 1;
	}

	template<class T>
	std::string ArgScalar<T>::GetHelpText() const {
		return GetHelpTextWithMessage("Takes a generic type : ");
	}

	template<>
	int ArgScalar<bool>::SetValue(const std::string& optarg);

	template<>
	ArgObject::Accept_t ArgScalar<bool>::AcceptsArgument(const std::string& arg) const;

	template<>
	int ArgScalar<bool>::PassArgument(const std::string& arg, const std::string& opt, const bool with_opt);

	template<>
	std::string ArgScalar<bool>::GetHelpText() const;

	template<>
	std::string ArgScalar<std::string>::GetHelpText() const;

	template<>
	std::string ArgScalar<char>::GetHelpText() const;

	template<>
	std::string ArgScalar<unsigned char>::GetHelpText() const;

	template<>
	std::string ArgScalar<short>::GetHelpText() const;

	template<>
	std::string ArgScalar<unsigned short>::GetHelpText() const;

	template<>
	std::string ArgScalar<int>::GetHelpText() const;

	template<>
	std::string ArgScalar<unsigned int>::GetHelpText() const;

	template<>
	std::string ArgScalar<long>::GetHelpText() const;

	template<>
	std::string ArgScalar<unsigned long>::GetHelpText() const;

	template<>
	std::string ArgScalar<long long>::GetHelpText() const;

	template<>
	std::string ArgScalar<unsigned long long>::GetHelpText() const;

	template<>
	std::string ArgScalar<float>::GetHelpText() const;

	template<>
	std::string ArgScalar<double>::GetHelpText() const;

	template<>
	std::string ArgScalar<long double>::GetHelpText() const;
}

#endif
#ifndef ARGPARSE_ArgVector_HDR
#define ARGPARSE_ArgVector_HDR

#include <sstream>


namespace ArgParse {
	template<class T>
	class ArgVector : public Argument {
		public:
			ArgVector(const std::string& call_name, const std::string& help_text, std::vector<T>* value, const Req_t required, bool* was_defined = nullptr);
			~ArgVector() {}

			virtual int SetValue(const std::string& optarg) __attribute__((warn_unused_result));

			virtual ArgObject::Accept_t AcceptsArgument(const std::string& arg __attribute__((unused))) const __attribute__((warn_unused_result));
			virtual int PassArgument(const std::string& arg, const std::string& opt, const bool with_opt) __attribute__((warn_unused_result));
			virtual size_t AmountOfData() const;

			virtual std::string GetHelpText() const;

			virtual void AppendType(std::stringstream& ss) const {
				ss << "Vector : ";
			}

			virtual ArgObject::Mode_t GetMode() const {
				return ArgObject::Multiple;
			}

		private:
			std::vector<T>* value;
	};

	template<class T>
	ArgVector<T>::ArgVector(const std::string& call_name, const std::string& help_text, std::vector<T>* value, const Req_t required, bool* was_defined) : Argument(call_name, help_text, required, was_defined) {
		this->value = value;
	}

	template<class T>
	int ArgVector<T>::SetValue(const std::string& optarg) {
		std::stringstream ss(optarg);
		T temp_val;
		if(!(ss >> temp_val)) {
			ArgParseMessageError("There was an error reading values from the string stream.\n");
			return -1;
		}
		if(ss.eof()) {
			SetDefined(true);
			this->value->push_back(temp_val);
			return 0;
		} else {
			ArgParseMessageError("String stream has some sort of error.\n");
			return -2;
		}
	}

	template<class T>
	ArgObject::Accept_t ArgVector<T>::AcceptsArgument(const std::string& arg) const {
		if(DebugLevel > 1) {
			MessageStandardPrint("Testing the argument (%s)\n", arg.c_str());
		}
		size_t pos;
		bool result = DoesAnArgumentMatch(pos, arg);
		if(!result) {
			return ArgObject::No;
		}

		return ArgObject::WithMultipleArg;
	}

	template<class T>
	int ArgVector<T>::PassArgument(const std::string& arg, const std::string& opt, const bool with_opt) {
		size_t pos;
		bool result = DoesAnArgumentMatch(pos, arg);
		if(!result) {
			return ArgObject::NotAccepted;
		}

		if(!with_opt) {
			ArgParseMessageError("A non-boolean argument must have a value!\n");
			SetMessage("A non-boolean argument must have a value!\n");
			return ArgObject::Error;
		}

		if(SetValue(opt) < 0) {
			ArgParseMessageError("There was a problem setting the value of the option! Arg(%s) Opt(%s)\n", arg.c_str(), opt.c_str());
			return ArgObject::Error;
		}
		return ArgObject::Accepted;
	}

	template<class T>
	size_t ArgVector<T>::AmountOfData() const {
		if(!WasDefined()) {
			return 0;
		}
		return this->value->size();
	}

	template<class T>
	std::string ArgVector<T>::GetHelpText() const {
		return GetHelpTextWithMessage("Takes a generic type : ");
	}

	template<>
	int ArgVector<bool>::SetValue(const std::string& optarg);

	template<>
	ArgObject::Accept_t ArgVector<bool>::AcceptsArgument(const std::string& arg) const;

	template<>
	int ArgVector<bool>::PassArgument(const std::string& arg, const std::string& opt, const bool with_opt);

	template<>
	std::string ArgVector<bool>::GetHelpText() const;

	template<>
	std::string ArgVector<std::string>::GetHelpText() const;

	template<>
	std::string ArgVector<char>::GetHelpText() const;

	template<>
	std::string ArgVector<unsigned char>::GetHelpText() const;

	template<>
	std::string ArgVector<short>::GetHelpText() const;

	template<>
	std::string ArgVector<unsigned short>::GetHelpText() const;

	template<>
	std::string ArgVector<int>::GetHelpText() const;

	template<>
	std::string ArgVector<unsigned int>::GetHelpText() const;

	template<>
	std::string ArgVector<long>::GetHelpText() const;

	template<>
	std::string ArgVector<unsigned long>::GetHelpText() const;

	template<>
	std::string ArgVector<long long>::GetHelpText() const;

	template<>
	std::string ArgVector<unsigned long long>::GetHelpText() const;

	template<>
	std::string ArgVector<float>::GetHelpText() const;

	template<>
	std::string ArgVector<double>::GetHelpText() const;

	template<>
	std::string ArgVector<long double>::GetHelpText() const;
}

#endif


namespace ArgParse {
	//Need to forward declare this..
	class ArgGroup;

	class ArgObjContainer {
		public:
			ArgObjContainer(ArgObjContainer* parent = nullptr) {
				this->parent = parent;
			}
			virtual ~ArgObjContainer();

			template<class T>
			void AddArgument(const std::string& call_name, const std::string& help_text, T* argument, const ArgObject::Req_t required = ArgObject::Optional, bool* was_defined = nullptr) {
				Argument* the_argument = (Argument*) new ArgScalar<T>(call_name, help_text, argument, required, was_defined);
				AddArgument(the_argument);
			}

			template<class T>
			void AddArgument(const std::string& call_name, const std::string& help_text, std::vector<T>* argument, const ArgObject::Req_t required = ArgObject::Optional, bool* was_defined = nullptr) {
				Argument* the_argument = (Argument*) new ArgVector<T>(call_name, help_text, argument, required, was_defined);
				AddArgument(the_argument);
			}

			ArgGroup* AddArgGroup(const std::string& title, const std::string& help_text, const ArgObject::Mode_t mode = ArgObject::None, const ArgObject::Req_t required = ArgObject::Optional);
			ArgGroup* AddInclusiveArgGroup(const std::string& title, const std::string& help_text, const ArgObject::Mode_t mode = ArgObject::None, const ArgObject::Req_t required = ArgObject::Optional);
			ArgGroup* AddExclusiveArgGroup(const std::string& title, const std::string& help_text, const ArgObject::Mode_t mode = ArgObject::None, const ArgObject::Req_t required = ArgObject::Optional);

		private:
			void CheckName(const std::string& call_name, ArgObjContainer* parent);
			void AddArgument(Argument* argument);
			void AddArgGroupObject(ArgGroup* arggroup);
			void AddArgObject(ArgObject* object);

		protected:
			ArgObjContainer* parent;
			std::vector<ArgObject*> objects;
	};
}

#endif

namespace ArgParse {
	class ArgGroup : public ArgObject , public ArgObjContainer {
		public:
			ArgGroup(const std::string& title, const std::string& help_text, const ArgObject::Mode_t mode = ArgObject::None, const ArgObject::Req_t required = ArgObject::Optional, ArgObjContainer* parent = nullptr) : ArgObject(help_text, required) , ArgObjContainer(parent) {
				this->mode = mode;
				this->title = title;
			}
			~ArgGroup() {
			}

			virtual bool IsConfigured() const __attribute__((warn_unused_result));
			virtual size_t AmountOfData() const;
			virtual ArgObject::Accept_t AcceptsArgument(const std::string& arg) const __attribute__((warn_unused_result));
			virtual ArgObject::Pass_t PassArgument(const std::string& arg, const std::string& opt, const bool with_opt) __attribute__((warn_unused_result));

			virtual ArgObject::State_t State(bool quiet = false) const __attribute__((warn_unused_result));

			virtual std::string GetHelpText() const;

			virtual bool CheckSubObjects() const __attribute__((warn_unused_result));

			virtual bool CheckDataConsistency() const __attribute__((warn_unused_result));

			std::string GetGroupHelpText(const std::string& name) const;

			const std::string& GetTitle() const {
				return this->title;
			}

			virtual ArgObject::Mode_t GetMode() const {
				return this->mode;
			}

		private:
			ArgObject::Mode_t mode;
			std::string title;
	};
}

#endif
#ifndef ARGPARSE_ArgParser_HDR
#define ARGPARSE_ArgParser_HDR

#include <vector>
#include <string>


namespace ArgParse {
	class ArgParser : public ArgObjContainer {
		public:
			ArgParser(const std::string& help_intro);
			~ArgParser();

			void PrintHelp();

			bool HelpPrinted() const {
				return help_printed;
			}

			int ParseArgs(int& argc, char**& argv) __attribute__((warn_unused_result));

			std::string ArgsToString(int& argc, char**& argv);

			static int EatArgument(int& argc, char**& argv, const int i = 1) __attribute__((warn_unused_result));
			static bool SplitArg(std::string& arg, std::string& opt, const std::string argument);
			int ObjectIdxAcceptingArgument(const std::string& arg __attribute__((unused))) const __attribute__((warn_unused_result));

		private:
			std::string help_intro;
			bool help_printed;
	};
}

#endif

#endif

namespace ArgParse {
	template<>
	int ArgScalar<bool>::SetValue(const std::string& optarg) {
		if (optarg != "") {
			ArgParseMessageError("Trying to set the value of a bool with a non-empty string!\n");
			return -1;
		}
		*value = true;
		SetDefined(true);
		return 0;
	}

	template<>
	ArgObject::Accept_t ArgScalar<bool>::AcceptsArgument(const std::string& arg) const {
		if(DebugLevel > 1) {
			MessageStandardPrint("Testing the argument (%s)\n", arg.c_str());
		}
		size_t pos;
		bool result = DoesAnArgumentMatch(pos, arg);
		if(!result) {
			return ArgObject::No;
		}

		return ArgObject::WithoutArg;
	}

	template<>
	int ArgScalar<bool>::PassArgument(const std::string& arg, const std::string& opt, const bool with_opt) {
		size_t pos;
		bool result = DoesAnArgumentMatch(pos, arg);
		if(!result) {
			return ArgObject::NotAccepted;
		}

		if(with_opt) {
			ArgParseMessageError("A boolean argument cannot accept a value!\n");
			SetMessage("A boolean argument cannot accept a value!\n");
			return ArgObject::Error;
		}

		if(opt.size() != 0) {
			ArgParseMessageError("You told me you didn't pass an option, but you did!\n");
			SetMessage("You told me you didn't pass an option, but you did!\n");
			return ArgObject::Error;
		}

		if(SetValue("") < 0) {
			return ArgObject::Error;
		}
		return ArgObject::Accepted;
	}

	template<>
	std::string ArgScalar<bool>::GetHelpText() const {
		return GetHelpTextWithMessage("Takes no argument : ");
	}

	template<>
	std::string ArgScalar<std::string>::GetHelpText() const {
		return GetHelpTextWithMessage("Takes a string : ");
	}

	template<>
	std::string ArgScalar<char>::GetHelpText() const {
		return GetHelpTextWithMessage("Takes a character : ");
	}

	template<>
	std::string ArgScalar<unsigned char>::GetHelpText() const {
		return GetHelpTextWithMessage("Takes an unsigned character : ");
	}

	template<>
	std::string ArgScalar<short>::GetHelpText() const {
		return GetHelpTextWithMessage("Takes a short : ");
	}

	template<>
	std::string ArgScalar<unsigned short>::GetHelpText() const {
		return GetHelpTextWithMessage("Takes an unsigned short : ");
	}

	template<>
	std::string ArgScalar<int>::GetHelpText() const {
		return GetHelpTextWithMessage("Takes an integer : ");
	}

	template<>
	std::string ArgScalar<unsigned int>::GetHelpText() const {
		return GetHelpTextWithMessage("Takes an unsigned integer : ");
	}

	template<>
	std::string ArgScalar<long>::GetHelpText() const {
		return GetHelpTextWithMessage("Takes a long : ");
	}

	template<>
	std::string ArgScalar<unsigned long>::GetHelpText() const {
		return GetHelpTextWithMessage("Takes an unsigned long : ");
	}

	template<>
	std::string ArgScalar<long long>::GetHelpText() const {
		return GetHelpTextWithMessage("Takes a long long : ");
	}

	template<>
	std::string ArgScalar<unsigned long long>::GetHelpText() const {
		return GetHelpTextWithMessage("Takes an unsigned long long : ");
	}

	template<>
	std::string ArgScalar<float>::GetHelpText() const {
		return GetHelpTextWithMessage("Takes a float : ");
	}

	template<>
	std::string ArgScalar<double>::GetHelpText() const {
		return GetHelpTextWithMessage("Takes a double : ");
	}

	template<>
	std::string ArgScalar<long double>::GetHelpText() const {
		return GetHelpTextWithMessage("Takes a long double : ");
	}
}
#include <cstdarg>


namespace ArgParse {
	std::string currentMessage = "";
	FILE* STDOUT_Channel = stdout;
	FILE* STDERR_Channel = stderr;
	int DebugLevel = -1;
	bool Color = true;

	const std::string& GetMessage() {
		return currentMessage;
	}

	void SetMessage(const std::string& message) {
		currentMessage = message;
	}

	int _vscprintf (const char* format, va_list pargs) {
		int retval;
		va_list argcopy;
		va_copy(argcopy, pargs);
		retval = vsnprintf(nullptr, 0, format, argcopy);
		va_end(argcopy);
		return retval;
	}

	void SetMessage(const char* format, ...) {
		va_list argptr;
		va_start(argptr, format);
		int the_length = _vscprintf(format, argptr)+1;
		char buffer[the_length];
		vsprintf(buffer, format, argptr);
		va_end(argptr);
		currentMessage = buffer;
	}

	void SetSTDOUTChannel(FILE* stdout_channel) {
		STDOUT_Channel = stdout_channel;
	}

	void SetSTDERRChannel(FILE* stderr_channel) {
		STDERR_Channel = stderr_channel;
	}

	void MessageStandardPrint(const char* format, ...) {
		if(STDOUT_Channel != nullptr) {
			va_list argptr;
			va_start(argptr, format);
			vfprintf(STDOUT_Channel, format, argptr);
			va_end(argptr);
		}
	}

	void MessageErrorPrint(const char* format, ...) {
		if(STDERR_Channel != nullptr) {
			va_list argptr;
			va_start(argptr, format);
			vfprintf(STDERR_Channel, format, argptr);
			va_end(argptr);
		}
	}
}

namespace ArgParse {
	template<>
	int ArgVector<bool>::SetValue(const std::string& optarg) {
		if (optarg != "") {
			ArgParseMessageError("Trying to set the value of a bool with a non-empty string!\n");
			return -1;
		}
		value->push_back(true);
		SetDefined(true);
		return 0;
	}

	template<>
	ArgObject::Accept_t ArgVector<bool>::AcceptsArgument(const std::string& arg) const {
		if(DebugLevel > 1) {
			MessageStandardPrint("Testing the argument (%s)\n", arg.c_str());
		}
		size_t pos;
		bool result = DoesAnArgumentMatch(pos, arg);
		if(!result) {
			return ArgObject::No;
		}

		return ArgObject::WithoutArg;
	}

	template<>
	int ArgVector<bool>::PassArgument(const std::string& arg, const std::string& opt, const bool with_opt) {
		size_t pos;
		bool result = DoesAnArgumentMatch(pos, arg);
		if(!result) {
			return ArgObject::NotAccepted;
		}

		if(with_opt) {
			ArgParseMessageError("A boolean argument cannot accept a value!\n");
			SetMessage("A boolean argument cannot accept a value!\n");
			return ArgObject::Error;
		}

		if(opt.size() != 0) {
			ArgParseMessageError("You told me you didn't pass an option, but you did!\n");
			SetMessage("You told me you didn't pass an option, but you did!\n");
			return ArgObject::Error;
		}

		if(SetValue("") < 0) {
			return ArgObject::Error;
		}
		return ArgObject::Accepted;
	}

	template<>
	std::string ArgVector<bool>::GetHelpText() const {
		return GetHelpTextWithMessage("Takes no argument : ");
	}

	template<>
	std::string ArgVector<std::string>::GetHelpText() const {
		return GetHelpTextWithMessage("Takes a string : ");
	}

	template<>
	std::string ArgVector<char>::GetHelpText() const {
		return GetHelpTextWithMessage("Takes a character : ");
	}

	template<>
	std::string ArgVector<unsigned char>::GetHelpText() const {
		return GetHelpTextWithMessage("Takes an unsigned character : ");
	}

	template<>
	std::string ArgVector<short>::GetHelpText() const {
		return GetHelpTextWithMessage("Takes a short : ");
	}

	template<>
	std::string ArgVector<unsigned short>::GetHelpText() const {
		return GetHelpTextWithMessage("Takes an unsigned short : ");
	}

	template<>
	std::string ArgVector<int>::GetHelpText() const {
		return GetHelpTextWithMessage("Takes an integer : ");
	}

	template<>
	std::string ArgVector<unsigned int>::GetHelpText() const {
		return GetHelpTextWithMessage("Takes an unsigned integer : ");
	}

	template<>
	std::string ArgVector<long>::GetHelpText() const {
		return GetHelpTextWithMessage("Takes a long : ");
	}

	template<>
	std::string ArgVector<unsigned long>::GetHelpText() const {
		return GetHelpTextWithMessage("Takes an unsigned long : ");
	}

	template<>
	std::string ArgVector<long long>::GetHelpText() const {
		return GetHelpTextWithMessage("Takes a long long : ");
	}

	template<>
	std::string ArgVector<unsigned long long>::GetHelpText() const {
		return GetHelpTextWithMessage("Takes an unsigned long long : ");
	}

	template<>
	std::string ArgVector<float>::GetHelpText() const {
		return GetHelpTextWithMessage("Takes a float : ");
	}

	template<>
	std::string ArgVector<double>::GetHelpText() const {
		return GetHelpTextWithMessage("Takes a double : ");
	}

	template<>
	std::string ArgVector<long double>::GetHelpText() const {
		return GetHelpTextWithMessage("Takes a long double : ");
	}
}
#include <sstream>
#include <cerrno>
#include <climits>
#include <cmath>


namespace ArgParse {
	Argument::Argument(const std::string& call_name, const std::string& help_text, const Req_t required, bool* was_defined) : ArgObject(help_text, required) {
		this->call_names = GetCallNames(call_name);
		if(was_defined == nullptr) {
			this->defined = new bool;
			this->responsible_for_defined = true;
			*(this->defined) = false;
		} else {
			this->responsible_for_defined = false;
			this->defined = was_defined;
		}
	}

	Argument::~Argument() {
		if(this->responsible_for_defined) {
			delete this->defined;
		}
	}

	std::vector<std::string> Argument::GetCallNames(const std::string& combined_names) {
		std::vector<std::string> answer;
		const size_t buffer_size = combined_names.size()+1;
		char temp_buffer[buffer_size];
		strcpy(temp_buffer, combined_names.c_str());
		char* token = strtok(temp_buffer, "/");
		do {
			if(token == NULL) {
				break;
			}
			if(DebugLevel > 0) {
				MessageStandardPrint("Found name: (%s)\n", token);
			}
			answer.push_back(std::string(token));
			token = strtok(NULL, "/");
		} while (true);
		return answer;
	}

	std::string Argument::GetName(size_t i) const {
		if(call_names.size() > i) {
			return call_names[i];
		} else {
			return "";
		}
	}

	void Argument::PrepareHelpText(std::stringstream& ss) const {
		for(size_t i=0; i<call_names.size(); ++i) {
			ss << GetName(i) << " ";
			if(i < call_names.size()-1) {
				ss << "/ ";
			}
		}
		ss << ": ";
	}

	void Argument::AppendType(std::stringstream& ss) const {
		ss << " Generic Type : ";
	}

	std::string Argument::GetHelpTextWithMessage(const std::string& message) const {
		std::stringstream ss;
		PrepareHelpText(ss);
		ss << message;
		AppendType(ss);
		ss << GetHelp();
		return ss.str();
	}

	std::string Argument::GetHelpText() const {
		return GetHelpTextWithMessage("Takes a generic argument : ");
	}

	bool Argument::DoesAnArgumentMatch(size_t& position, const std::string& arg) const {
		size_t i=0;
		for(; i<call_names.size(); ++i) {
			if(DebugLevel > 1) {
				MessageStandardPrint("checking if call name (%s) matches.\n", call_names[i].c_str());
			}
			//if(DebugLevel > 3) {
			//	MessageStandardPrint("position: %lu arg: (%s)\n", position, arg.c_str());
			//}
			if(arg == call_names[i]) {
				break;
			}
		}
		if (i != call_names.size()) {
			position = i;
			return true;
		} else {
			return false;
		}
	}

	ArgObject::State_t Argument::State(bool quiet) const {
		if (IsRequired()) {
			if(!WasDefined()) {
				if (!quiet) {
					ArgParseMessageError("The argument (%s) needs to be defined.\n", GetName().c_str());
					SetMessage("The argument (%s) needs to be defined.\n", GetName().c_str());
				}
				return ArgObject::NotReady;
			} else {
				return ArgObject::Ready;
			}
		} else {
			if (WasDefined()) {
				return ArgObject::Defined;
			} else {
				return ArgObject::NotDefined;
			}
		}
	}
}
#ifndef ARGPARSE_ArgExclusiveGroup_HDR
#define ARGPARSE_ArgExclusiveGroup_HDR


namespace ArgParse {
	class ArgExclusiveGroup : public ArgGroup {
		public:
			ArgExclusiveGroup(const std::string& title, const std::string& help_text, const ArgObject::Mode_t mode = ArgObject::None, const ArgObject::Req_t required = ArgObject::Optional, ArgObjContainer* parent = nullptr);

			virtual bool IsConfigured() const __attribute__((warn_unused_result));

			virtual std::string GetHelpText() const;

			virtual bool CheckSubObjects() const __attribute__((warn_unused_result));
	};
}

#endif

namespace ArgParse {
	ArgExclusiveGroup::ArgExclusiveGroup(const std::string& title, const std::string& help_text, const ArgObject::Mode_t mode, const ArgObject::Req_t required, ArgObjContainer* parent) : ArgGroup(title, help_text, mode, required, parent) {
	}

	bool ArgExclusiveGroup::IsConfigured() const {
		//If any of it's sub objects aren't configured, it isn't configured.
		for(size_t i = 0; i < objects.size(); ++i) {
			if(!objects[i]->IsConfigured()) {
				return false;
			}
		}
		if(GetMode() != ArgObject::None) {
			//Now check for consistency with group mode
			for(size_t i=0; i< objects.size(); ++i) {
				if (objects[i]->GetMode() != GetMode()) {
					ArgParseMessageError("A sub argument(%i) of this container(%s) doesn't have the right mode! All elments must have matching modes. object(%s) versus container(%s)\n", i, this->GetTitle().c_str(), TranslateMode(objects[i]->GetMode()), TranslateMode(GetMode()));
					SetMessage("A sub argument(%i) of this container(%s) doesn't have the right mode! All elments must have matching modes. object(%s) versus container(%s)\n", i, this->GetTitle().c_str(), TranslateMode(objects[i]->GetMode()), TranslateMode(GetMode()));
					return false;
				}
			}
		}
		//Now check for consistency with requirement
		for(size_t i=0; i< objects.size(); ++i) {
			if(objects[i]->GetRequired() != GetRequired()) {
				ArgParseMessageError("A sub argument(%i) of this container(%s) doesn't have the same requirement criteria! object(%i) versus container(%i)\n", i, this->GetTitle().c_str(), TranslateReq(objects[i]->GetRequired()), TranslateReq(GetRequired()));
				SetMessage("A sub argument(%i) of this container(%s) doesn't have the same requirement criteria! object(%i) versus container(%i)\n", i, this->GetTitle().c_str(), TranslateReq(objects[i]->GetRequired()), TranslateReq(GetRequired()));
				return false;
			}
		}
		return true;
	}

	std::string ArgExclusiveGroup::GetHelpText() const {
		return GetGroupHelpText("Exclusive");
	}

	bool ArgExclusiveGroup::CheckSubObjects() const {
		if(GetRequired()) {
			int num_ready = 0;
			for(size_t i=0; i<objects.size(); ++i) {
				ArgObject::State_t obj_state = objects[i]->State(true);
				if(obj_state == ArgObject::Ready) {
					num_ready += 1;
				}
			}
			if (num_ready == 0) {
				ArgParseMessageError("No arguments of the required exclusive group (%s) were ready.\n", GetTitle().c_str());
				SetMessage("No arguments of the required exclusive group (%s) were ready.\n", GetTitle().c_str());
				return false;
			}
			if (num_ready != 1) {
				ArgParseMessageError("Only a single argument in the exclusive group (%s) should be defined.\n", GetTitle().c_str());
				SetMessage("Only a single argument in the exclusive group (%s) should be defined.\n", GetTitle().c_str());
				return false;
			}
		} else {
			int num_defined = 0;
			for(size_t i=0; i<objects.size(); ++i) {
				ArgObject::State_t obj_state = objects[i]->State(true);
				if((obj_state == ArgObject::Defined)||(obj_state == ArgObject::Ready)) {
					num_defined += 1;
				}
			}
			if(num_defined > 1) {
				ArgParseMessageError("Only a single argument in the exclusive group (%s) should be defined.\n", GetTitle().c_str());
				SetMessage("Only a single argument in the exclusive group (%s) should be defined.\n", GetTitle().c_str());
				return false;
			}
		}
		return true;
	}
}
#ifndef ARGPARSE_ArgInclusiveGroup_HDR
#define ARGPARSE_ArgInclusiveGroup_HDR


namespace ArgParse {
	class ArgInclusiveGroup : public ArgGroup {
		public:
			ArgInclusiveGroup(const std::string& title, const std::string& help_text, const ArgObject::Mode_t mode = ArgObject::None, const ArgObject::Req_t required = ArgObject::Optional, ArgObjContainer* parent = nullptr);

			virtual bool IsConfigured() const __attribute__((warn_unused_result));
			virtual size_t AmountOfData() const;

			virtual std::string GetHelpText() const;

			virtual bool CheckSubObjects() const __attribute__((warn_unused_result));

			virtual bool CheckDataConsistency() const __attribute__((warn_unused_result));
	};
}

#endif

namespace ArgParse {
	ArgInclusiveGroup::ArgInclusiveGroup(const std::string& title, const std::string& help_text, const ArgObject::Mode_t mode, const ArgObject::Req_t required, ArgObjContainer* parent) : ArgGroup(title, help_text, mode, required, parent) {
	}

	bool ArgInclusiveGroup::IsConfigured() const {
		//If any of it's sub objects aren't configured, it isn't configured.
		for(size_t i = 0; i < objects.size(); ++i) {
			if(!objects[i]->IsConfigured()) {
				return false;
			}
		}
		if(GetMode() != ArgObject::None) {
			//Now check for consistency with group mode
			for(size_t i=0; i< objects.size(); ++i) {
				Mode_t the_mode = objects[i]->GetMode();
				if (the_mode != GetMode()) {
					ArgParseMessageError("A sub argument(%i) of this container(%s) doesn't have the right mode! All elments must have matching modes. object(%s) versus container(%s)\n", i, this->GetTitle().c_str(), TranslateMode(the_mode), TranslateMode(GetMode()));
					SetMessage("A sub argument(%i) of this container(%s) doesn't have the right mode! All elments must have matching modes. object(%s) versus container(%s)\n", i, this->GetTitle().c_str(), TranslateMode(the_mode), TranslateMode(GetMode()));
					return false;
				}
			}
		}
		//Now check for consistency with requirement
		for(size_t i=0; i< objects.size(); ++i) {
			if(objects[i]->GetRequired() != GetRequired()) {
				ArgParseMessageError("A sub argument(%i) of this container(%s) doesn't have the same requirement criteria! object(%s) versus container(%s)\n", i, this->GetTitle().c_str(), TranslateReq(objects[i]->GetRequired()), TranslateReq(GetRequired()));
				SetMessage("A sub argument(%i) of this container(%s) doesn't have the same requirement criteria! object(%s) versus container(%s)\n", i, this->GetTitle().c_str(), TranslateReq(objects[i]->GetRequired()), TranslateReq(GetRequired()));
				return false;
			}
		}
		return true;
	}

	size_t ArgInclusiveGroup::AmountOfData() const {
		return objects[0]->AmountOfData();
	}

	std::string ArgInclusiveGroup::GetHelpText() const {
		return GetGroupHelpText("Inclusive");
	}

	bool ArgInclusiveGroup::CheckSubObjects() const {
		if(GetRequired()) {
			for(size_t i=0; i<objects.size(); ++i) {
				ArgObject::State_t obj_state = objects[i]->State(true);
				if(obj_state != ArgObject::Ready) {
					ArgParseMessageError("A sub argument(%i) of the group (%s) wasn't ready. (%s)\n", i, GetTitle().c_str(), TranslateState(obj_state));
					SetMessage("A sub argument(%i) of the group (%s) wasn't ready. (%s)\n", i, GetTitle().c_str(), TranslateState(obj_state));
					return false;
				}
			}
		} else {
			ArgObject::State_t obj_state = objects[0]->State(true);
			bool defined = false;
			if((obj_state == ArgObject::Defined)||(obj_state == ArgObject::Ready)) {
				defined = true;
			}
			for(size_t i=1; i<objects.size(); ++i) {
				obj_state = objects[i]->State(true);
				if (defined) {
					if((obj_state == ArgObject::NotDefined)||(obj_state == ArgObject::NotReady)) {
						ArgParseMessageError("All sub arguments of the group (%s) must either be defined or not defined. (%s)\n", GetTitle().c_str(), TranslateState(obj_state));
						SetMessage("All sub arguments of the group (%s) must either be defined or not defined. (%s)\n", GetTitle().c_str(), TranslateState(obj_state));
						return false;
					}
				} else {
					if((obj_state == ArgObject::Defined)||(obj_state == ArgObject::Ready)) {
						ArgParseMessageError("All sub arguments of the group (%s) must either be defined or not defined. (%s)\n", GetTitle().c_str(), TranslateState(obj_state));
						SetMessage("All sub arguments of the group (%s) must either be defined or not defined. (%s)\n", GetTitle().c_str(), TranslateState(obj_state));
						return false;
					}
				}
			}
		}
		return true;
	}

	bool ArgInclusiveGroup::CheckDataConsistency() const {
		if((GetMode() == ArgObject::Multiple)||(GetMode() == ArgObject::Single)) {
			//Check that everybody has the same amount of data.
			size_t the_size = objects[0]->AmountOfData();
			for(size_t i=1; i<objects.size(); ++i) {
				if(objects[i]->AmountOfData() != the_size) {
					ArgParseMessageError("You need to pass the same number of arguments for each argument in an inclusive group.\n");
					SetMessage("You need to pass the same number of arguments for each argument in an inclusive group.\n");
					return false;
				}
			}
		}
		return true;
	}
}

namespace ArgParse {
	const ArgObject::Mode_t ArgObject::Single = 0;
	const ArgObject::Mode_t ArgObject::Multiple = 1;
	const ArgObject::Mode_t ArgObject::None = -1;

	const ArgObject::Req_t ArgObject::Required = true;
	const ArgObject::Req_t ArgObject::Optional = false;

	const ArgObject::Accept_t ArgObject::No = 0;
	const ArgObject::Accept_t ArgObject::WithSingleArg = 1;
	const ArgObject::Accept_t ArgObject::WithMultipleArg = 4;
	const ArgObject::Accept_t ArgObject::WithoutArg = 2;
	const ArgObject::Accept_t ArgObject::GroupName = 3;

	const ArgObject::Pass_t ArgObject::NotAccepted = 0;
	const ArgObject::Pass_t ArgObject::Accepted = 1;
	const ArgObject::Pass_t ArgObject::Error = -1;

	const ArgObject::State_t ArgObject::NotReady = -1;
	const ArgObject::State_t ArgObject::Defined = 0;
	const ArgObject::State_t ArgObject::NotDefined = 1;
	const ArgObject::State_t ArgObject::Ready = 2;

	ArgObject::~ArgObject() {
	}

	const char* ArgObject::TranslateState(const State_t i) {
		if(i == NotReady) {
			return "NotReady";
		} else if (i == Defined) {
			return "Defined";
		} else if (i == NotDefined) {
			return "NotDefined";
		} else if (i == Ready) {
			return "Ready";
		} else {
			return "Undefined State_t";
		}
	}

	const char* ArgObject::TranslateReq(const Req_t i) {
		if(i == Required) {
			return "Required";
		} else if (i == Optional) {
			return "Optional";
		} else {
			return "Undefined Req_t";
		}
	}

	const char* ArgObject::TranslateAccept(const Accept_t i) {
		if(i == No) {
			return "No";
		} else if (i == WithSingleArg) {
			return "WithSingleArg";
		} else if (i == WithMultipleArg) {
			return "WithMultipleArg";
		} else if (i == WithoutArg) {
			return "WithoutArg";
		} else if (i == GroupName) {
			return "GroupName";
		} else {
			return "Undefined Accept_t";
		}
	}

	const char* ArgObject::TranslateMode(const Mode_t i) {
		if(i == Single) {
			return "Single";
		} else if (i == Multiple) {
			return "Multiple";
		} else if (i == None) {
			return "None";
		} else {
			return "Undefined Mode_t";
		}
	}

	const char* ArgObject::TranslatePass(const Pass_t i) {
		if(i == NotAccepted) {
			return "NotAccepted";
		} else if (i == Accepted) {
			return "Accepted";
		} else if (i == Error) {
			return "Error";
		} else {
			return "Undefined Pass_t";
		}
	}
}

namespace ArgParse {
	ArgObjContainer::~ArgObjContainer() {
		for(size_t i=0; i<objects.size(); ++i) {
			delete objects[i];
		}
	}

	ArgGroup* ArgObjContainer::AddArgGroup(const std::string& title, const std::string& help_text, const ArgObject::Mode_t mode, const ArgObject::Req_t required) {
		ArgGroup* the_group = new ArgGroup(title, help_text, mode, required, this);
		AddArgGroupObject(the_group);
		return the_group;
	}

	ArgGroup* ArgObjContainer::AddInclusiveArgGroup(const std::string& title, const std::string& help_text, const ArgObject::Mode_t mode, const ArgObject::Req_t required) {
		ArgGroup* the_group = (ArgGroup*) new ArgInclusiveGroup(title, help_text, mode, required, this);
		AddArgGroupObject(the_group);
		return the_group;
	}

	ArgGroup* ArgObjContainer::AddExclusiveArgGroup(const std::string& title, const std::string& help_text, const ArgObject::Mode_t mode, const ArgObject::Req_t required) {
		ArgGroup* the_group = (ArgGroup*) new ArgExclusiveGroup(title, help_text, mode, required, this);
		AddArgGroupObject(the_group);
		return the_group;
	}

	void ArgObjContainer::CheckName(const std::string& call_name, ArgObjContainer* parent) {
		if(parent == nullptr) {
			for(size_t i=0; i<objects.size(); ++i) {
				ArgObject::Accept_t accept = objects[i]->AcceptsArgument(call_name);
				if(accept != ArgObject::No) {
					if(accept == ArgObject::GroupName) {
						ArgParseMessageError("The argument (%s) conflicts with a group name!\n", call_name.c_str());
						SetMessage("The argument (%s) conflicts with a group name!\n", call_name.c_str());
					} else {
						ArgParseMessageError("The argument (%s) has already been defined!\n", call_name.c_str());
						SetMessage("The argument (%s) has already been defined!\n", call_name.c_str());
					}
					exit(-1);
				}
			}
		} else {
			parent->CheckName(call_name, parent->parent);
		}
	}

	void ArgObjContainer::AddArgument(Argument* argument) {
		//Check that the name hasn't been defined already
		for(size_t i=0; i<argument->GetNum(); ++i) {
			CheckName(argument->GetName(i), parent);
		}
		AddArgObject((ArgObject*) argument);
	}

	void ArgObjContainer::AddArgGroupObject(ArgGroup* arggroup) {
		//Check that the name hasn't been defined already
		CheckName(arggroup->GetTitle(), parent);
		AddArgObject((ArgObject*) arggroup);
	}

	void ArgObjContainer::AddArgObject(ArgObject* object) {
		objects.push_back(object);
	}
}
#include <sstream>
#include <cstring>


namespace ArgParse {
	ArgParser::ArgParser(const std::string& help_intro) : ArgObjContainer() {
		this->help_printed = false;
		this->help_intro = help_intro;
	}

	ArgParser::~ArgParser() {
	}

	void ArgParser::PrintHelp() {
		ArgParseMessagePrint("%s\n", help_intro.c_str());
		ArgParseMessagePrint("The Argument listing is in columns separated by colons ':'.\n");
		ArgParseMessagePrint("The columns give the following information.\n");
		ArgParseMessagePrint("1 - Names the argument must be refered by.\n");
		ArgParseMessagePrint("2 - What type of argument it takes or if it takes no argument.\n");
		ArgParseMessagePrint("3 - Whether the argument should be specified a single time (Scalar) Or can be specified multiple times (Vector)\n");
		ArgParseMessagePrint("4 - Help text for that argument.\n");
		ArgParseMessagePrint("--- Arguments ---\n");
		ArgParseMessagePrint("-h / -? / --help : Takes no argument : Vector : Print this help text.\n");
		for(size_t i=0; i<objects.size(); ++i) {
			ArgParseMessagePrint("%s\n", objects[i]->GetHelpText().c_str());
		}
		help_printed = true;
	}

	int ArgParser::ParseArgs(int& argc, char**& argv) {
		std::string command_line = ArgsToString(argc, argv);
		//Check that the options are configured.
		for(size_t i=0; i<objects.size(); ++i) {
			if(!objects[i]->IsConfigured()) {
				ArgParseMessageError("There was a problem parsing the arguments. The command line was (%s)\n", command_line.c_str());
				return -1;
			}
		}
		//We start at 1 because the zeroth argument is the program name.
		int arg_i=1;
		int obj_idx_accepting_multiple_args = -1;
		std::string multiple_args_arg = "";
		while(arg_i<argc) {
			if(DebugLevel > 0) {
				MessageStandardPrint("Argument is: (%s)\n", argv[arg_i]);
			}
			std::string arg;
			std::string opt;
			bool split_arg = SplitArg(arg, opt, argv[arg_i]);
			if(arg == std::string("--")) {
				if(!split_arg) {
					//We need to eat this variable and then quit.
					if(DebugLevel > 2) {
						MessageStandardPrint("Eating an argument.\n");
					}
					if(EatArgument(argc, argv, arg_i) < 0) {
						MessageStandardPrint("There was a problem eating an argument!\n");
						ArgParseMessageError("There was a problem parsing the arguments. The command line was (%s)\n", command_line.c_str());
						return -1;
					}
					if(DebugLevel > 2) {
						MessageStandardPrint("Finished eating an argument.\n");
					}
					break;
				}
			}
			if(arg == std::string("-h")) {
				if(DebugLevel > 0) {
					MessageStandardPrint("Help discovered!\n", argv[arg_i]);
				}
				PrintHelp();
				return 0;
			}
			if(arg == std::string("--help")) {
				if(DebugLevel > 0) {
					MessageStandardPrint("Help discovered!\n", argv[arg_i]);
				}

				PrintHelp();
				return 0;
			}
			if(arg == std::string("-?")) {
				if(DebugLevel > 0) {
					MessageStandardPrint("Help discovered!\n", argv[arg_i]);
				}

				PrintHelp();
				return 0;
			}
			int accepting_obj_idx = ObjectIdxAcceptingArgument(arg);
			if(accepting_obj_idx < 0) {
				if (obj_idx_accepting_multiple_args >= 0) {
					if(DebugLevel > 5) {
						ArgParseMessageDebug("Arg (%s) isn't a known argument, but there is an argument (%s) which is taking multiple arguments. Passing this arg to that argument\n", argv[arg_i], multiple_args_arg.c_str());
					}

					ArgObject::Pass_t passed = objects[obj_idx_accepting_multiple_args]->PassArgument(multiple_args_arg, argv[arg_i], true);
					if(passed == ArgObject::Error) {
						ArgParseMessageError("There was a problem parsing the arguments. The command line was (%s)\n", command_line.c_str());
						return -2;
					}
					if(passed == ArgObject::NotAccepted) {
						ArgParseMessageError("The argument did not accept what we passed it! this shouldn't happen!\n");
						SetMessage("The argument did not accept what we passed it! this shouldn't happen!\n");
						ArgParseMessageError("There was a problem parsing the arguments. The command line was (%s)\n", command_line.c_str());
						return -3;
					}
				} else {
					ArgParseMessageError("The argument (%s) does not exist.\n", arg.c_str());
					SetMessage("The argument (%s) does not exist.\n", arg.c_str());
					ArgParseMessageError("There was a problem parsing the arguments. The command line was (%s)\n", command_line.c_str());
					return -2;
				}
			} else {
				ArgObject::Accept_t accepted = objects[accepting_obj_idx]->AcceptsArgument(arg);
				if(DebugLevel > 5) {
					ArgParseMessageDebug("Accepting object %lu gave acceptance message (%s)\n", accepting_obj_idx, ArgObject::TranslateAccept(accepted));
				}

				if (obj_idx_accepting_multiple_args < 0) {
					if (DebugLevel > 5) {
						ArgParseMessageDebug("Clearing object accepting multiple args.\n");
					}
					//Reset the multiarg variables
					obj_idx_accepting_multiple_args = -1;
					multiple_args_arg = "";
				}

				if (accepted == ArgObject::WithoutArg) {
					if(DebugLevel > 5) {
						ArgParseMessageDebug("object %lu accepts the argument without an option\n", accepting_obj_idx);
					}
					if(DebugLevel > 0) {
						MessageStandardPrint("Doesn't need a value\n");
					}
					if (split_arg) {
						ArgParseMessageError("The argument (%s) doesn't take a value!\n", arg.c_str());
						ArgParseMessageError("There was a problem parsing the arguments. The command line was (%s)\n", command_line.c_str());
						return -3;
					}
					if(DebugLevel > 1) {
						MessageStandardPrint("Setting Value\n");
					}
					ArgObject::Pass_t passed = objects[accepting_obj_idx]->PassArgument(arg, opt, false);
					if(passed == ArgObject::Error) {
						ArgParseMessageError("There was a problem passing the argument (%s) to (%s)\n", opt.c_str(), arg.c_str());
						ArgParseMessageError("There was a problem parsing the arguments. The command line was (%s)\n", command_line.c_str());
						return -4;
					}
					if(passed == ArgObject::NotAccepted) {
						ArgParseMessageError("The argument did not accept what we passed it! this shouldn't happen!\n");
						SetMessage("The argument did not accept what we passed it! this shouldn't happen!\n");
						ArgParseMessageError("There was a problem parsing the arguments. The command line was (%s)\n", command_line.c_str());
						return -5;
					}
					if(DebugLevel > 1) {
						MessageStandardPrint("Finished Setting Value\n");
					}
				} else if ((accepted == ArgObject::WithSingleArg)||(accepted == ArgObject::WithMultipleArg)) {
					if(DebugLevel > 5) {
						ArgParseMessageDebug("object %lu accepts the argument with an option\n", accepting_obj_idx);
					}
					if(DebugLevel > 0) {
						MessageStandardPrint("Needs a value\n");
					}
					if(!split_arg) {
						if(DebugLevel > 2) {
							MessageStandardPrint("Eating an argument.\n");
						}
						if(EatArgument(argc, argv, arg_i) < 0) {
							ArgParseMessageError("There was a problem eating an argument!\n");
							SetMessage("There was a problem eating an argument!\n");
							ArgParseMessageError("There was a problem parsing the arguments. The command line was (%s)\n", command_line.c_str());
							return -1;
						}
						if(DebugLevel > 2) {
							MessageStandardPrint("Finished eating an argument.\n");
						}
						opt = std::string(argv[arg_i]);
					}
					if(DebugLevel > 1) {
						MessageStandardPrint("Setting Value (%s)\n", opt.c_str());
					}
					ArgObject::Pass_t passed = objects[accepting_obj_idx]->PassArgument(arg, opt, true);
					if(passed == ArgObject::Error) {
						ArgParseMessageError("There was a problem parsing the arguments. The command line was (%s)\n", command_line.c_str());
						return -2;
					}
					if(passed == ArgObject::NotAccepted) {
						ArgParseMessageError("The argument did not accept what we passed it! this shouldn't happen!\n");
						SetMessage("The argument did not accept what we passed it! this shouldn't happen!\n");
						ArgParseMessageError("There was a problem parsing the arguments. The command line was (%s)\n", command_line.c_str());
						return -3;
					}
					if (accepted == ArgObject::WithMultipleArg) {
						obj_idx_accepting_multiple_args = accepting_obj_idx;
						multiple_args_arg = arg;
						if(DebugLevel > 5) {
							ArgParseMessageDebug("Set the object currently accepting multiple args as %i\n", obj_idx_accepting_multiple_args);
						}
					}
					if(DebugLevel > 1) {
						MessageStandardPrint("Finished Setting Value\n");
					}
				} else {
					ArgParseMessageError("Something strange was returned from AcceptsArgument!\n");
					SetMessage("Something strange was returned from AcceptsArgument!\n");
					ArgParseMessageError("There was a problem parsing the arguments. The command line was (%s)\n", command_line.c_str());
					return -6;
				}
			}
			if(DebugLevel > 2) {
				MessageStandardPrint("Eating an argument.\n");
			}
			if(EatArgument(argc, argv, arg_i) < 0) {
				ArgParseMessageError("There was a problem eating an argument!\n");
				SetMessage("There was a problem eating an argument!\n");
				ArgParseMessageError("There was a problem parsing the arguments. The command line was (%s)\n", command_line.c_str());
				return -2;
			}
			if(DebugLevel > 2) {
				MessageStandardPrint("Finished eating an argument.\n");
			}
		}
		for(size_t i=0; i<objects.size(); ++i) {
			if(objects[i]->State() == ArgObject::NotReady) {
				ArgParseMessageError("One of the arguments wasn't ready!\n");
				SetMessage("One of the arguments wasn't ready!\n");
				ArgParseMessageError("There was a problem parsing the arguments. The command line was (%s)\n", command_line.c_str());
				return -3;
			}
		}
		return 0;
	}

	std::string ArgParser::ArgsToString(int& argc, char**& argv) {
		std::stringstream ss;
		for(int i=0; i<argc; ++i) {
			ss << argv[i];
			if(i < argc-1) {
				ss << " ";
			}
		}
		return ss.str();
	}

	int ArgParser::EatArgument(int& argc, char**& argv, const int i) {
		if (i >= argc) {
			return -1;
		}
		//shift arguments
		argc--;
		int j = i;
		while (j < argc) {
			argv[j] = argv[j+1];
			j++;
		}
		return 0;
	}

	bool ArgParser::SplitArg(std::string& arg, std::string& opt, const std::string argument) {
		const int normal = 0;
		const int quoted = 0;
		const int escaped = 0;
		int mode = normal;

		bool found_equals = false;
		size_t equals_position = 0;
		char quote_type;
		size_t i = 0;
		while (i < argument.size()) {
			if (mode == normal) {
				if (argument[i] == '=') {
					found_equals = true;
					equals_position = i;
					break;
				}
				if ((argument[i] == '\'')||(argument[i] == '"')) {
					mode = quoted;
					quote_type = argument[i];
				}
			} else if (mode == quoted) {
				if (argument[i] == quote_type) {
					mode = normal;
				}
				if (argument[i] == '\\') {
					mode = escaped;
				}
			} else if (mode == escaped) {
			}
			++i;
		}

		if (!found_equals) {
			arg = argument;
			return false;
		} else {
			arg = argument.substr(0, equals_position);
			opt = argument.substr(equals_position+1, argument.size()-equals_position-1);
			if(opt.size() == 0) {
				ArgParseMessageWarning("You passed an empty value to the argument (%s)! This could screw up the parsing of later arguments.\n", arg.c_str());
			}
			return true;
		}
	}


	int ArgParser::ObjectIdxAcceptingArgument(const std::string& arg __attribute__((unused))) const {
		for(size_t i=0; i<objects.size(); ++i) {
			if(objects[i]->AcceptsArgument(arg) != ArgObject::No) {
				return (int) i;
			}
		}
		return -1;
	}
}
#include <cstring>


namespace ArgParse {
	const char* basename(const char* filename) {
		const char* p = strrchr(filename, '/');
		return p ? p + 1 : filename;
	}
}
#include <sstream>


namespace ArgParse {
	bool ArgGroup::IsConfigured() const {
		//If any of it's sub objects aren't configured, it isn't configured.
		for(size_t i = 0; i < objects.size(); ++i) {
			if(!objects[i]->IsConfigured()) {
				return false;
			}
		}
		return true;
	}

	size_t ArgGroup::AmountOfData() const {
		for(size_t i=0; i<objects.size(); ++i) {
			size_t amount = objects[i]->AmountOfData();
			if(amount != 0) {
				return amount;
			}
		}
		return 0;
	}

	ArgObject::Accept_t ArgGroup::AcceptsArgument(const std::string& arg) const {
		if (DebugLevel > 5) {
			ArgParseMessageDebug("Checking if we accept an argument\n");
		}
		if (arg == GetTitle()) {
			return ArgObject::GroupName;
		}
		for(size_t i=0; i<this->objects.size(); ++i) {
			ArgObject::Accept_t accept = this->objects[i]->AcceptsArgument(arg);
			if (accept != ArgObject::No) {
				return accept;
			}
		}
		return ArgObject::No;
	}

	ArgObject::Pass_t ArgGroup::PassArgument(const std::string& arg, const std::string& opt, const bool with_opt) {
		for(size_t i=0; i < this->objects.size(); ++i) {
			ArgObject::Pass_t passed = this->objects[i]->PassArgument(arg, opt, with_opt);
			if(passed != ArgObject::NotAccepted) {
				return passed;
			}
		}
		return ArgObject::NotAccepted;
	}

	ArgObject::State_t ArgGroup::State(bool quiet __attribute__((unused))) const {
		//Check that everybody is ready.
		if(!CheckSubObjects()) {
			return ArgObject::NotReady;
		}
		//Check that the data is consistent
		if(!CheckDataConsistency()) {
			return ArgObject::NotReady;
		}
		return ArgObject::Ready;
	}

	std::string ArgGroup::GetGroupHelpText(const std::string& name) const {
		std::stringstream ss;
		ss << "--- Begin ";
		if(name.size() != 0) {
			ss << name;
		}
		ss << "Group " << title << " ---" << std::endl;
		ss << GetHelp() << std::endl;
		if(DebugLevel > 5) {
			ArgParseMessageDebug("There are %lu objects in this group.\n", this->objects.size());
		}
		for(size_t i=0; i< this->objects.size(); ++i) {
			ss << this->objects[i]->GetHelpText() << std::endl;
		}
		ss << "--- End ";
		if(name.size() != 0) {
			ss << name;
		}
		ss << "Group " << title << " ---" << std::endl;
		return ss.str();

	}

	std::string ArgGroup::GetHelpText() const {
		return GetGroupHelpText("");
	}

	bool ArgGroup::CheckSubObjects() const {
		for(size_t i=0; i<objects.size(); ++i) {
			State_t ready = objects[i]->State();
			if(ready == ArgObject::NotReady) {
				ArgParseMessageError("A sub argument(%i) of the group (%s) wasn't ready. (%s)\n", i, GetTitle().c_str(), ArgObject::TranslateState(ready));
				SetMessage("A sub argument(%i) of the group (%s) wasn't ready. (%s)\n", i, GetTitle().c_str(), ArgObject::TranslateState(ready));
				return false;
			}
		}
		return true;
	}

	bool ArgGroup::CheckDataConsistency() const {
		return true;
	}
}
#endif
