# include "../inc/ft_ping.h"

extern t_ping g_ping;

void parse_hostname(char *target) {
    struct addrinfo hints;
    struct addrinfo *res;

    bzero(&hints, sizeof hints);
    hints.ai_family = AF_UNSPEC;  
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(target, NULL, &hints, &res) != 0) {
        printf("ping: %s: Name or service not known\n", target);
		exit(1);
    }
    void *addr;
    g_ping.res = (struct sockaddr_in *)res->ai_addr;
    addr = &((g_ping.res)->sin_addr);
    inet_ntop(res->ai_family, addr, g_ping.ip, sizeof g_ping.ip);
    freeaddrinfo(res);
}

int parser(int ac, char **av) {
	char *arg;
	int i = 1;
	int j = 1;

	if (ac < 2) {
		printf("Usage: ping [-hv] destination\n");
		return (1);
	}

	while (i < ac) {
		arg = av[i];
		if (arg[0] == '-') {
			if (strlen(arg) < 2) {
				printf("ping: -: Name or service not known\n");
				return (1);
			}
			while (arg[j] != '\0') {
				if (arg[j] == 'h') {
				printf("Usage: ping [-hv] destination\n");
					return (1);
				}
				else if (arg[j] == 'v')
					g_ping.v_option = 1;
				else {
					printf("ping: option requires an argument -- '%c'\n", arg[j]);
					printf("Usage: ping [-hv] destination\n");
					return (1);
				}
				j++;
			}
		}
		else {
			snprintf(g_ping.target, sizeof(g_ping.target), "%s", arg);
			parse_hostname(arg);
		}
		i++;
	}
	return (0);
}