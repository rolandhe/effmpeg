//
// Created by hexiufeng on 2023/11/5.
//

#ifndef EFFMPEGG_INTERNAL_H
#define EFFMPEGG_INTERNAL_H
#ifdef DEBUG
#   define ff_dlog(ctx, ...) av_log(ctx, AV_LOG_DEBUG, __VA_ARGS__)
#else
#   define ff_dlog(ctx, ...) do { if (0) av_log(ctx, AV_LOG_DEBUG, __VA_ARGS__); } while (0)
#endif
#endif //EFFMPEGG_INTERNAL_H
