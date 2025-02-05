#ifndef SRC_TCPSERVER_H_
#define SRC_TCPSERVER_H_

#include <thread>
#include <vector>
#include "ServerSocket.h"
#include "ConnectionHandler.h"

namespace pr {

void handleClient(Socket scli) {
    int fd = scli.getFD();
    int message;
    recv(fd, &message, sizeof message, 0);
    std::cout << "Reçu " << message << std::endl;
    int reply = 1337;
    send(fd, &reply, sizeof reply, 0);
    scli.close();
}

// un serveur TCP, la gestion des connections est déléguée
class TCPServer {
	ServerSocket * ss; // la socket d'attente si elle est instanciee
	ConnectionHandler * handler; // le gestionnaire de session passe a la constru
	// a completer
public :
	TCPServer(ConnectionHandler * handler): ss(nullptr),handler(handler) {}
	// Tente de creer une socket d'attente sur le port donné
	bool startServer (int port) {
        ServerSocket ss(port);
        std::vector<std::thread> threads;
        while(true) {
            Socket cli = ss.accept();
            std::thread t(handleClient, cli);
            threads.push_back(t);
        }
    }

	// stoppe le serveur
	void stopServer () {
        ss->close();
    }
};

} // ns pr

#endif /* SRC_TCPSERVER_H_ */
