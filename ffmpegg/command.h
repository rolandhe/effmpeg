//
// Created by hexiufeng on 2023/10/31.
//

#ifndef EFFMPEGG_PARSE_CMD_H
#define EFFMPEGG_PARSE_CMD_H

#include <libavutil/opt.h>
#include <libavutil/dict.h>
#include "cmd_options.h"
#include "cmd_util.h"



#define QUICK_DURATION_INCORRECT -101

int show_hwaccels();
void init_ffmpeg();
int run_ffmpeg_cmd(char * cmd);

int quick_duration(char *cmd, int64_t * p_duration);

#endif //EFFMPEGG_PARSE_CMD_H
