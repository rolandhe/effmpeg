#include <stdio.h>
#include "ffmpegg/command.h"
#include "ffmpegg/base64.h"
#include <libavutil/opt.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>

typedef struct Param {
    int pipe_id;
    char *file_name;
} Param;

void * read_data(void * param){
    Param *p = (Param*)param;
    FILE * out = fopen(p->file_name,"wb");
    char buf[8196];
    int read_count = 0;
    while (1){

        int ssize = read(p->pipe_id,buf,8196);
        read_count += ssize;
        av_log(NULL,AV_LOG_INFO,"read %d data,all %d\n",ssize,read_count);
        if(ssize == 0){
            fclose(out);
            break;
        }
        if(ssize < 0){
            fclose(out);
            av_log(NULL,AV_LOG_INFO,"met error\n");
            break;
        }

        fwrite(buf,1,ssize,out);
    }

}

char * get_data(char * src_file,int pipe_id){
    FILE * f = fopen(src_file,"rb");
    int BUFF_SIZE = 1024*1024;
    char * buff = malloc(BUFF_SIZE);
    memset(buff,0,BUFF_SIZE);

    char out_file[1024];
    memset(out_file,0,sizeof (out_file));
    sprintf(out_file," -f adts pipe:%d",pipe_id);
    int out_len = strlen(out_file);

    int len = fread(buff,1,BUFF_SIZE,f);
    fclose(f);
    int b_len = Base64encode_len(len);

    char * pro = "ffmpeg -f mp3 -i data:content/type;base64,";
    int pro_len = strlen(pro);
    int data_len = b_len + pro_len + out_len ;
    char * data = malloc(data_len);
    memset(data,0,data_len);
    strcpy(data,pro);
    int encode_len = Base64encode(data + pro_len,buff,len);
    free(buff);
    strcpy(data + pro_len + encode_len-1,out_file);
    return data;
}

void test_leak(int count){
    char cmd[1024] = {0};
    char trace_id[256]={0};
    printf("to start............\n");
    getchar();
    for(int i = 0; i < count;i++){
        sprintf(cmd,"ffmpeg -i s95.mp3 mmm_%d.aac",i);
        sprintf(trace_id,"trace_id_index_%d",i);
        int ret = run_ffmpeg_cmd(trace_id,cmd);
        printf("ret:%d\n",ret);
    }
    printf("to end............\n");
    getchar();
}

void run_once(){
    char * cmd1 = "ffmpeg -i s95.mp3 mmm.aac";
    int ret = run_ffmpeg_cmd("trace_99800m",cmd1);
    printf("%d\n",ret);
}

void quick(){
    // -f s16le -i
    char * cmd = "ffmpeg -i /home/xiufeng/github/effmpeg/vvx.m4a";

    int64_t duration;
    int ret = quick_duration("trace_id-10999",cmd, &duration);
    printf("%d\n",ret);
//    if(ret == -101){
//        printf("get incorrect duration,try to convert");
//        ret = run_ffmpeg_cmd(cmd1);
//        if(ret == 0){
//            ret = quick_duration(cmd,&duration);
//        }
//    }
}



void pip_trans();
int extractAudioFromWebm(char * input,int64_t input_len,char **output,int *output_len);

void extractWebm(){

    char * src_file = "/home/xiufeng/github/effmpeg/vvx.m4a";
    FILE * f = fopen(src_file,"rb");
    fseek(f,0,SEEK_END);
    int size = ftell(f);

    char * buffer = malloc(size);
    fseek(f,0,SEEK_SET);
    int n = fread(buffer,1,size,f);
    fclose(f);

    char * output = NULL;
    int out_len = 0;
    int ret = extractAudioFromWebm(buffer,n,&output,&out_len);

    char * dest_file = "/Users/hexiufeng/Downloads/webm/last-be.opus";


    if(output){
        FILE * out = fopen(dest_file,"wb");
        fwrite(output,1,out_len,out);
        fclose(out);
        free(output);
    }

    printf("ret:%d\n",ret);

}

int main() {
    init_ffmpeg();
    extractWebm();
//    quick();
//    test_leak(10000);
//    show_hwaccels();
//

//    run_once();
//    pip_trans();
    return 0;
}

void pip_trans(){
    int pip[] = {0,0};
    int ret = pipe(pip);

//    for(int i = 0; i< 1;i++){
//    memset(cmd,1,sizeof (cmd));
    Param param;
    param.pipe_id = pip[0];
    param.file_name = "out.aac";
    pthread_t pthread;
    pthread_create(&pthread, NULL, read_data, &param);
//    memset(cmd,0,sizeof (cmd));

    char *pipe_cmd = get_data("s95.mp3",pip[1]);

//    sprintf(cmd,"ffmpeg -i s95.mp3 -f adts pipe:%d", pip[1]);

    char * trace_id = "trace_id_00998888";
    ret = run_ffmpeg_cmd(trace_id,pipe_cmd);
    av_log(NULL,AV_LOG_INFO,"tid=%s,task %d, ret:%d\n",trace_id,0,ret);
//    }

    close(pip[1]);
    free(pipe_cmd);
    pthread_join(pthread, NULL);
}


typedef struct {
    char *data;
    int size;
} MemBuffer;

int resample16k(char * input,int input_len,int *out_len){
    if(input_len % 6 != 0) {
        return -1;
    }
    int16_t * in = (int16_t*)input;
    int all_size = input_len / 2;
    int index = 0;
    for (int i = 0;i < all_size;i += 3){
        in[index++] = in[i];
    }
    *out_len = index * 2;
    return 0;
}


int extractAudioFromWebm(char * input,int64_t input_len,char **output,int *output_len){
    const char * fmt = "ffmpeg -f webm -i filemem:0x%lx -vn -f s16be filemem:0x%lx.pcm";
//    const char * fmt = "ffmpeg -i filemem:0x%lx -vn -f s16le /Users/hexiufeng/Downloads/webm/xxoo.pcm";
//    const char * fmt = "ffmpeg -i /Users/hexiufeng/Downloads/webm/recorder.webm -vn -f s16le /Users/hexiufeng/Downloads/webm/xxoo.pcm";
    MemBuffer memInput = {
            .data = input,
            .size = input_len,
    };

    MemBuffer memOut = {
            .data = NULL,
            .size = 0,
    };

    *output = NULL;
    *output_len = 0;

    char cmd[2048] = {0};

    void * pi = &memInput;
    int64_t ii_value = (int64_t) pi;

    sprintf(cmd,fmt,ii_value,(int64_t)&memOut);

    int ret = run_ffmpeg_cmd("extract-trace-1000",cmd);
    if(ret != 0){
        if(memOut.data){
            free(memOut.data);
        }
        return ret;
    }

    ret = resample16k(memOut.data,memOut.size,&memOut.size);
    if(ret != 0){
        if(memOut.data){
            free(memOut.data);
        }
        return ret;
    }

    memInput.data = memOut.data;
    memInput.size = memOut.size;

    memOut.data = NULL;
    memOut.size = 0;
    const char * opus_format = "ffmpeg -f s16be -ar 16000 -i filemem:0x%lx -c:a libopus -ar 16000 -frame_duration 20 filemem:0x%lx.opus";
    sprintf(cmd,opus_format,ii_value,(int64_t)&memOut);

    ret = run_ffmpeg_cmd("extract-trace-1000",cmd);
    free(memInput.data);
    if(ret != 0){
        if(memOut.data){
            free(memOut.data);
        }
        return ret;
    }

    *output = memOut.data;
    *output_len = memOut.size;

    return 0;
}