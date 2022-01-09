#include "wrappers.h"
#include "analyzer.h"

void print_gistogram(std::map<char, size_t> const &dictionary) {
	for (auto &elem : (dictionary)) {
		std::cout << "symbol \"" << elem.first << "\"\t";
		for (size_t i = 0; i < elem.second; ++i) {
			std::cout << "#";
		}
		std::cout << " (" << elem.second << ")" << std::endl;
	}
}

void 
count_symbols(std::map<char, size_t> &dictionary, std::string const &str) 
{
	for (auto &pchar : str) {
		if (pchar != '\n' || pchar != '\t' || 
			pchar != ' '  || pchar != '\0')    

		{

			auto find_pair = dictionary.find(pchar);
            if (find_pair != dictionary.end())
                find_pair->second++;
            dictionary.insert(std::pair<char, size_t> (pchar, 1));
		}
	} 
}

int main(int argc, char const *argv[]) {
	int client_socket = Socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in adr = {0};
	adr.sin_family = AF_INET;
	adr.sin_port = htons(PORT);

	Inet_pton(AF_INET, "127.0.0.1", &adr.sin_addr);

	int res_connect = Connect(client_socket, 
							 (struct sockaddr* ) &adr, sizeof(adr));

	while (true) {

		printf("Select request number by issue type:\n"
			"1 - analyze the text for the occurrence of characters\n"
			"2 - long number calculator (+ - operations are supported)\n"
			"0 - exit\n\n");
		
		char task = fgetc(stdin);

		std::string send_msg;
		send_msg[0] = task;

		if (task == '1') {

			printf("Enter the path to the file\n\nrequest:\t");
			getchar();
			std::getline(std::cin, send_msg);
			send_msg = "1" + std::to_string(getpid()) + " " + send_msg;

		}

		else if (task == '2') {

			printf("Enter the math expression in the format:"
				"\nlong_number operator long_number\n\nrequest:\t");
			getchar();
			std::getline(std::cin, send_msg);
			send_msg = "2" + send_msg;

		}

		else if (task == '0') {

			printf("you were disconnected\n");
			break;

		}

		else {

			printf("no such task number exists. select a task from the proposed\n");
			continue;

		}

		ssize_t count_send = Send(client_socket, send_msg.c_str(), MSG_SIZE, 0);

		char recv_msg[MSG_SIZE];

		ssize_t count_read = Recv(client_socket, recv_msg, MSG_SIZE, 0);

		if (task == '2')
			printf("reply:\t\t%s\n\n", recv_msg);
		if (task == '1') {
			printf("reply:\t\t%s\n", recv_msg);
			if (strncmp(recv_msg, "Done", 4) == 0) {
				char stat_file[10];
				sprintf(stat_file, "%d", (int) getpid());
				print_file_gistogram(stat_file);
			}
			
			printf("\n");
		}

		recv_msg[0] = '\0';
		send_msg.clear();

	}

	close(client_socket);
	return 0;
}