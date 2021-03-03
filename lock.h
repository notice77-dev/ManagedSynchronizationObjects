#pragma once

#include <tchar.h>
#include <exception>
#include <string>

#include <Windows.h>

namespace Common
{
	/////////////////////////////////////////////////////////////////////////////////
	// define the lock object here
	/////////////////////////////////////////////////////////////////////////////////
#define LOCK_NAME(NAME)			\
	NAME(ALLOCATOR_LOCK)		\
	NAME(USER_LOCK)				\

	/////////////////////////////////////////////////////////////////////////////////
	// macro of lock template argument
	/////////////////////////////////////////////////////////////////////////////////
#define DO_DESCRIPTION(e) L#e,
#define DO_ENUM(e) e,

	// defined of convert enum to string
	static const wchar_t* lockNameDescriptions[] = { LOCK_NAME(DO_DESCRIPTION) };

	// defined enum of LockName
	namespace Lock
	{
		enum LN
		{
			LOCK_NAME(DO_ENUM)
		};
	}

	/////////////////////////////////////////////////////////////////////////////////

	class InitException : public std::exception
	{
	private:
		std::string message;

	public:
		explicit InitException(const std::string& message)
			: message(message)
		{
		}

		const char* what() const noexcept override
		{
			return message.c_str();
		}
	};

	class Uncopyable
	{
	protected:
		Uncopyable() {};

	private:
		Uncopyable(const Uncopyable&);
		Uncopyable& operator=(const Uncopyable&);
	};

	// CriticalSection
	template<Lock::LN NAME>
	class CSLockObj : private Uncopyable
	{
	public:
		explicit CSLockObj(const size_t spinCount = 4000)
		{
			if (::InitializeCriticalSectionAndSpinCount(&m_cs, static_cast<DWORD>(spinCount)) == FALSE)
				throw InitException("InitializeCriticalSectionAndSpinCount fail");
		};

		~CSLockObj(void)
		{
			::DeleteCriticalSection(&m_cs);
		};

		static const LPCRITICAL_SECTION GetSyncObj()
		{
			static CSLockObj<NAME> syncObj;

			return &syncObj.m_cs;
		};

	private:
		CRITICAL_SECTION m_cs;
	};

	template<Lock::LN NAME>
	class CSLock : private Uncopyable
	{
	public:
		explicit CSLock(bool isLock)
			: m_rCnt(0)
		{
			if (isLock == true)
			{
				::EnterCriticalSection(CSLockObj<NAME>::GetSyncObj());
				++m_rCnt;
			}
		};

		~CSLock(void)
		{
			while (m_rCnt)
			{
				::LeaveCriticalSection(CSLockObj<NAME>::GetSyncObj());
				--m_rCnt;
			}
		};

		static const wchar_t* Name(void) { return lockNameDescriptions[NAME]; };

		void ManualLock(void) const
		{
			::EnterCriticalSection(CSLockObj<NAME>::GetSyncObj());
			++m_rCnt;
		};

		void ManualUnlock(void) const
		{
			if (m_rCnt > 0)
			{
				::LeaveCriticalSection(CSLockObj<NAME>::GetSyncObj());
				--m_rCnt;
			}
		};

		bool TryLock(void) const
		{
			if (::TryEnterCriticalSection(CSLockObj<NAME>::GetSyncObj()) == true)
			{
				++m_rCnt;
				return true;
			}

			return false;
		};

		void TryUnLock(void) const
		{
			if (m_rCnt > 0)
			{
				::LeaveCriticalSection(CSLockObj<NAME>::GetSyncObj());
				--m_rCnt;
			}
		};

	private:
		size_t m_rCnt;
	};

} // end of namespace Common