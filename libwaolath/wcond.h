#pragma once

typedef struct wcond {
	pthread_cond_t cond;
	pthread_mutex_t m;
} wcond_t;
