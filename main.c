#include <stdio.h>
#include "ffmpegg/command.h"
#include <unistd.h>

int main() {
    init_ffmpeg();
//    show_hwaccels();
//    char * cmd = "ffmpeg -f s16le -i pp.pcm";
    char * cmd1 = "ffmpeg -f s16le -ar 16000 -ac 1 -i pp.pcm mmm.mp3";
    int ret = run_ffmpeg_cmd(cmd1);
//    int64_t duration;
//    int ret = quick_duration(cmd, &duration);
//    if(ret == -101){
//        printf("get incorrect duration,try to convert");
//        ret = run_ffmpeg_cmd(cmd1);
//        if(ret == 0){
//            ret = quick_duration(cmd,&duration);
//        }
//    }
    printf("%d\n",ret);
//    printf("%ld\n",duration);
    return 0;
}

