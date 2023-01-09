#include <iostream>
#include <windows.h>
#include <string>
#include <thread>
#include <libloaderapi.h>

using namespace std;

void get_proc_id(const char* window_title, DWORD& process_id)
{
	GetWindowThreadProcessId(FindWindow(NULL, window_title), &process_id);
}

void error(const char* error_title, const char* error_message)
{
	MessageBox(NULL, error_message, error_title, NULL);
	exit(-1);
}

bool file_exists(string file_name)
{
	struct stat buffer;
	return (stat(file_name.c_str(), & buffer) == 0);

}

int main()
{
	/*
	int x = MessageBox(FindWindowA("Block Games Client Injector", NULL), "Inject Block Games Client?", "Block Games Client Injector", MB_OKCANCEL);

	if (x == IDCANCEL)
	{
		return -1;
	}
	*/
	DWORD proc_id = NULL;
	char dll_path[MAX_PATH];
	const char* dll_name = "Analyzer.dll"; 
	const char* window_title = "OrbusVR"; 

	//####
	if (!file_exists(dll_name))
	{
		error("Block Games Client Injector", "The file \"BlockGamesClient.dll\" could not be found.");
	}
	
	if (!GetFullPathName(dll_name, MAX_PATH, dll_path, nullptr))
	{
		error("Block Games Client Injector", "Failed to get path. Make sure you didnt move the .exe from the folder containing the DLL!");
	}

	get_proc_id(window_title, proc_id);
	if (proc_id == NULL)
	{
		error("Block Games Client Injector", "Failed to get process ID. Is Orbus running?");
	}
	//####

	HANDLE h_process = OpenProcess(PROCESS_ALL_ACCESS, NULL, proc_id);
	if (!h_process)
	{
		error("Block Games Client Injector", "Failed to open a handle to process.");
	}

	void* allocated_memory = VirtualAllocEx(h_process, nullptr, MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (!allocated_memory)
	{
		error("Block Games Client Injector", "Failed to allocate memory. Try restarting OrbusVR");
	}

	if (!WriteProcessMemory(h_process, allocated_memory, dll_path, MAX_PATH, nullptr))
	{
		error("Block Games Client Injector", "Failed to write process memory. Try restarting OrbusVR");
	}
	HANDLE h_thread = CreateRemoteThread(h_process, nullptr, NULL, LPTHREAD_START_ROUTINE(LoadLibraryA), allocated_memory, NULL, nullptr);
	if (!h_thread)
	{
		error("Block Games Client Injector", "Failed to create remote thread");
	}
	CloseHandle(h_process);
	VirtualFreeEx(h_process, allocated_memory, NULL, MEM_RELEASE);
	// MessageBox(0, "Injected Block Games Client Successfully", "Block Games Client Injector", 0);
}