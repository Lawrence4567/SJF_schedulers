#include <stdio.h>
#include <stdlib.h>
#include <time.h>
struct job {
    int start_time;           /*the time the job arrives*/
    int end_time;             /*the time the job finish*/
    int need_time;            /*the time the job need.*/
    int run_time;             /*the time the job is actually running*/
};
void sortByStartTime(struct job *jobs, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            /**
             * First sort by start_time,
             * if the start_time is the same, then compare the need_time.
             */
            if (jobs[j].start_time > jobs[j + 1].start_time ||
                    (jobs[j].start_time == jobs[j+1].start_time &&
                     jobs[j].need_time > jobs[j+1].need_time)) {
                struct job tmp;
                tmp = jobs[j];
                jobs[j] = jobs[j+1];
                jobs[j+1] = tmp;
            }
        }
    }
}
void sjf(struct job *jobs, int n) {
    int timestamps = 0;
    int i = 0, r = 1;
    int cnt = 0;
    printf("------------------------------ SJF ---------------------------\n");
    // find the shortest job except the jobs has been finished.
    while (cnt < n) {
        i = -1;
        for (int j = 0; j < n; j++) {
            if (jobs[j].end_time != -1) {
                continue;
            }
            if (timestamps < jobs[j].start_time) {
                if (i == -1) {
                    i = j;
                    timestamps = jobs[j].start_time;
                }
                break;
            }
            if ((i == -1) || (jobs[j].need_time - jobs[j].run_time < jobs[i].need_time - jobs[i].run_time)) {
                i = j;
            }
        }
        // the job i is running.
        int last = timestamps;
        jobs[i].run_time = jobs[i].need_time;
        timestamps += jobs[i].need_time;
        jobs[i].end_time = timestamps;
        cnt++;
        printf("%d - %d : job %d\n", last, timestamps, i);
    }
    int turnaround_time = 0;
    double turnaround_time_w = 0;
    int wait_time = 0;
    for (int j = 0; j < n; j++) {
        turnaround_time_w += (double )(jobs[j].end_time - jobs[j].start_time) / jobs[j].need_time;
        turnaround_time += jobs[j].end_time - jobs[j].start_time;
        wait_time += jobs[j].end_time - jobs[j].start_time - jobs[j].need_time;
    }

    printf("Turnaround time with weight : %lf\n", turnaround_time_w);
    printf("Turnaround time : %d\n", turnaround_time);
    printf("Wait time : %d\n", wait_time);
}
void sjf_preemptive(struct job *jobs, int n) {
    int timestamps = 0;
    int i = 0, r = 1;
    int cnt = 0;
    printf("------------------------SJF preemptive---------------------\n");
    while (cnt < n) {
        i = -1;
        // find the shortest job except the jobs has been finished.
        for (int j = 0; j < n; j++) {
            if (jobs[j].end_time != -1) {
                continue;
            }
            if (timestamps < jobs[j].start_time) {
                if (i == -1) {
                    i = j;
                    timestamps = jobs[j].start_time;
                }
                break;
            }
            if ((i == -1) || (jobs[j].need_time - jobs[j].run_time < jobs[i].need_time - jobs[i].run_time)) {
                i = j;
            }
        }
        // find the upcoming job.
        while (timestamps >= jobs[r].start_time) {
            r++;
        }
        //T is the gap time between now and the start time of the upcoming job.
        int t = jobs[r].start_time - timestamps;

        // time t is enough to finish the job i.
        int last = timestamps;
        if (jobs[i].need_time - jobs[i].run_time <= t) {

            jobs[i].end_time = timestamps + (jobs[i].need_time - jobs[i].run_time);
            timestamps = jobs[i].end_time;
            jobs[i].run_time = jobs[i].need_time;
            cnt++;
        // the next job is arrive while job i is running.
        } else {
            jobs[i].run_time += t;
            timestamps += t;
        }
        printf("%d - %d : job %d\n", last, timestamps, i);
    }
    int turnaround_time = 0;
    double turnaround_time_w = 0;
    int wait_time = 0;
    for (int j = 0; j < n; j++) {
        turnaround_time_w += (double )(jobs[j].end_time - jobs[j].start_time) / jobs[j].need_time;
        turnaround_time += jobs[j].end_time - jobs[j].start_time;
        wait_time += jobs[j].end_time - jobs[j].start_time - jobs[j].need_time;
    }

    printf("Turnaround time with weight : %lf\n", turnaround_time_w);
    printf("Turnaround time : %d\n", turnaround_time);
    printf("Wait time : %d\n", wait_time);
}

int main() {
    int n;

    printf("please input the num of jobs : ");
    scanf("%d", &n);
    // the last job is just a "sentry".
    struct job js[n + 1];
    js[n].start_time = 0;
    js[n].need_time = 0;
    srand((unsigned)time(NULL));
    for (int i = 0; i < n; i++) {
        js[i].start_time = rand() % 100 + 1;
        js[i].need_time = rand() % 20 + 1;
        js[i].run_time = 0;
        js[i].end_time = -1;
    }

    sortByStartTime(js, n);

    for (int i = 0; i < n; i++) {
        printf("start_time : %d, need_time : %d\n", js[i].start_time, js[i].need_time);
    }

    sjf(js, n);
    //reset run_time and end_time.
    for (int i = 0; i < n; i++) {
        js[i].run_time = 0;
        js[i].end_time = -1;
    }
    sjf_preemptive(js, n);
    return 0;
}
