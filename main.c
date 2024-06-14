
# include "ft_ping.h"

t_ping g_ping;

void parse_hostname(char *target) {
    struct addrinfo hints = { .ai_family = AF_UNSPEC, .ai_socktype = SOCK_STREAM }, *res;
    int status = 0;
    status = getaddrinfo(target, NULL, &hints, &res);
    if (status != 0) {
            printf("ping: unknown host\n");
        exit(1);
	}


    g_ping.res = (struct sockaddr_in *)res->ai_addr;
    inet_ntop(res->ai_family, &g_ping.res->sin_addr, g_ping.ip, sizeof(g_ping.ip));
    freeaddrinfo(res);
}


void handler(int signo) {
    if (signo == SIGINT) {
        printf("\n--- %s ping statistics ---\n", g_ping.target);
        printf("%d packets transmitted, %d received, %d%% packet loss\n", g_ping.seq, g_ping.received, (int)((float)(g_ping.seq - g_ping.received) / g_ping.seq) * 100);
        printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n", g_ping.min_rtt, g_ping.sum_rtt / g_ping.received, g_ping.max_rtt, g_ping.mdev_rtt * 10);
        close(g_ping.sockfd);
        exit(0);
    }
}

void init_socket() {
    if ((g_ping.sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0) {
		 perror("socket"); 
		 exit(1); 
	}
    g_ping.timeout = 1;
    struct timeval tv = { .tv_sec = 1, .tv_usec = 0 };
    if (setsockopt(g_ping.sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
           
		
        perror("setsockopt");
		exit(1); 
	}
    ping_loop();
}

int main(int ac, char **av) {
    if (ac < 2) { 
		printf("Usage: ping [-hv] destination\n"); 
		return 1; 
	}

    for (int i = 1; i < ac; i++) {
        char *arg = av[i];
        if (arg[0] == '-') {
            if (strlen(arg) < 2) { 
				printf("ping: -: Name or service not known\n");
				return 1;
			}
            for (int j = 1; arg[j] != '\0'; j++) {
                if (arg[j] == 'h') {
					printf("Usage: ping [-hv] destination\n"); 
					return 1;
				}
                else if (arg[j] == 'v') 
					g_ping.v_option = 1;
                else { 
					printf("ping: option requires an argument -- '%c'\n", arg[j]);
					printf("Usage: ping [-hv] destination\n"); return 1; 
				}
            }
        } else {
            snprintf(g_ping.target, sizeof(g_ping.target), "%s", arg);
            parse_hostname(arg);
        }
    }
    signal(SIGINT, handler);
    init_socket();
    return 0;
}
