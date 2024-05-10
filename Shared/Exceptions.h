#pragma once

#include <exception>
#include <stdexcept>
#include <locale> 
#include <codecvt>
#include <iostream>

namespace Moore
{
	namespace Shared
	{
		class IException : public std::runtime_error
		{
		protected:
			const std::string Message;
			
		public:
			enum class Location
			{
				BACKEND,
				FRONTEND,
				SHARED,
				CORE,
				BOOTSTRAPPER
			};

			virtual ~IException ()
			{}

			IException (const std::string msg, Location l)
				: std::runtime_error (msg.c_str ()), Loc (l), Message(msg)
			{}

			virtual char const* what () const noexcept override
			{
				return (char const*) (Message).c_str ();
			}

			virtual const std::string What () const noexcept
			{
				return Message;
			}

			friend std::wostream& operator<< (std::wostream& lhs, IException& rhs)
			{
				static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> Converter;
				lhs << L"An error has occured" << std::endl << Converter.from_bytes(rhs.What ()) << std::endl;
				return lhs;
			}

			friend std::ostream& operator<< (std::ostream& lhs, IException& rhs)
			{
				static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> Converter;
				lhs << "An error has occured" << std::endl << rhs.What () << std::endl;
				return lhs;
			}

		protected:
			const Location Loc;

		public:
			const Location Where () const noexcept
			{
				return Loc;
			}

		};

		class FrontendException : public IException
		{
		public:
			enum class FrontendComponent
			{
				INPUT_VALIDATION,
				OUTPUT,
				OPTION_VALIDATION
			};

			FrontendException (std::string Msg, FrontendComponent c)
				: IException (Msg, IException::Location::FRONTEND), _Comp(c)
			{}

			virtual char const* what () const noexcept override
			{
				return (char const*) (Message).c_str ();
			}

			virtual const std::string What () const noexcept
			{
				return Message;
			}

		public:
			const Location Where () const noexcept
			{
				return Loc;
			}

			FrontendComponent const _Comp;

		};

		class CoreException : public IException
		{
		public:
			enum class CoreComponent
			{
				STATE_VALIDATOR,
				DIALOGUE_FINDER
			};

			CoreException (std::string Msg, CoreComponent c)
				: IException (Msg, IException::Location::CORE), _Comp(c)
			{}

			virtual char const* what () const noexcept override
			{
				return (char const*) (Message).c_str ();
			}

			virtual const std::string What () const noexcept
			{
				return Message;
			}

		public:
			const Location Where () const noexcept
			{
				return Loc;
			}

			CoreComponent const _Comp;
		};

		class BackendException : public IException
		{
		public:
			enum class BackendComponent
			{
				FILE_ACCESS,
				DATALISTS_LOAD,
				DATALISTS_PARSE,
				DATALISTS_VALIDATE
			};

			BackendException (std::string Msg, BackendComponent c)
				: IException (Msg, IException::Location::BACKEND), _Comp (c)
			{}

			virtual char const* what () const noexcept override
			{
				return (char const*) (Message).c_str ();
			}

			virtual const std::string What () const noexcept
			{
				return Message;
			}

		public:
			const Location Where () const noexcept
			{
				return Loc;
			}

			BackendComponent const _Comp;
		};

		class BootstrapperException : public IException
		{
		public:
			enum class BootstrapperComponent
			{
				DIRECTORY_ENUMERATION,
				FILE_NOT_FOUND
			};

			BootstrapperException (std::string Msg, BootstrapperComponent c)
				: IException (Msg, IException::Location::BOOTSTRAPPER), _Comp (c)
			{}

			virtual char const* what () const noexcept override
			{
				return (char const*) (Message).c_str ();
			}

			virtual const std::string What () const noexcept
			{
				return Message;
			}

		public:
			const Location Where () const noexcept
			{
				return Loc;
			}

			BootstrapperComponent const _Comp;
		};
	}
}