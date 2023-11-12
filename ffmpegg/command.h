//
// Created by hexiufeng on 2023/10/31.
//

#ifndef EFFMPEGG_PARSE_CMD_H
#define EFFMPEGG_PARSE_CMD_H

#include <stdint.h>



#define QUICK_DURATION_INCORRECT -101

int show_hwaccels();
void init_ffmpeg();
int run_ffmpeg_cmd(char * trace_id,char * cmd);

int quick_duration(char * trace_id,char *cmd, int64_t * p_duration);

#endif //EFFMPEGG_PARSE_CMD_H
