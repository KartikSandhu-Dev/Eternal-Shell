#include "shell/signal.h"
#include "shell/shell.h"
#include "var/config.h"
#include "var/common.h"

#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void sig_shell_init(Shell *shell) {
	shell->joblist.capacity = JOBS_CAPACITY;
	shell->joblist.count = 0;

	shell->joblist.jobs = calloc(shell->joblist.capacity, sizeof(Job));

	if(shell->joblist.jobs == NULL) {
		perror("calloc");
		exit(EXIT_FAILURE);
	}

	signal(SIGINT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);

}

void sig_child_init(Shell *shell) {
	(void)shell;

	signal(SIGINT, SIG_DFL);
	signal(SIGTERM, SIG_DFL);
	signal(SIGTSTP, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGCONT, SIG_DFL);
	signal(SIGTTIN, SIG_DFL);
	signal(SIGTTOU, SIG_DFL);
}

void add_job(Shell *shell, int pid) {
	if(shell->joblist.count >= shell->joblist.capacity) {
		shell->joblist.capacity+=10;
		Job *tmp = realloc(shell->joblist.jobs, sizeof(Job) * shell->joblist.capacity);

		if(!tmp) {
			perror("shell_jobs_realloc");
			return;
		}

		shell->joblist.jobs = tmp;
	}

	Job job;
	job.id = shell->joblist.count + 1;
	job.pgid = pid;
	job.command = strdup(shell->shell_buffer);
	job.status = JOB_RUNNING;

	shell->joblist.jobs[shell->joblist.count] = job;
	shell->joblist.count++;

	printf("|%d| %d\n", job.id, job.pgid);
}

void check_jobs(Shell *shell) {
	int status;

	for(size_t i = 0; i < shell->joblist.count;) {
		Job *job = &shell->joblist.jobs[i];

		pid_t result = waitpid(-job->pgid, 
				&status, 
				WNOHANG | WUNTRACED | WCONTINUED);

		if(result == -1) {
    		perror("waitpid");
		}

		if(result == 0) {  i++; continue; }

		if(WIFEXITED(status) || WIFSIGNALED(status)) {
			job->status = JOB_DONE;

			printf("|%d| Done (%s)\n", job->id, job->command);

			clean_job(shell, i);
			continue;
		}

		if(WIFSTOPPED(status)) {
			job->status = JOB_STOPPED;
			printf("|%d| Stopped (%s)\n", job->id, job->command);
		}

		if(WIFCONTINUED(status)) {
			job->status = JOB_RUNNING;
		}

		i++;
	}

}

int find_job_index(Shell *shell, int id) {
	for(size_t i = 0; i < shell->joblist.count; i++) {
		if(shell->joblist.jobs[i].id == id) {
			return i;
		}
	}
	return -1;
}

void clean_job(Shell *shell, size_t index) {
	free(shell->joblist.jobs[index].command);

	for(size_t i = index; i + 1 < shell->joblist.count; i++) {
		shell->joblist.jobs[i] = shell->joblist.jobs[i+1];
	}

	shell->joblist.count--;
}