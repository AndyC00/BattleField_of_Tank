#ifndef  __LOGMANAGER_H  
#define  __LOGMANAGER_H

class LogManager
{
	// Member methods: 
public:
	static LogManager& GetInstance();
	static void DestroyInstance();

	void Log(const char* pcMessage);

protected:

private:
	LogManager();
	~LogManager();
	LogManager(const LogManager& logManager); 
	LogManager& operator=(const LogManager& logManager);

	// Member data: 
public:

protected:
	static LogManager* sm_pInstance;

private:

};

#endif //   __LOGMANAGER_H 
