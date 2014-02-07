// Mp4BoxDefine.h

#ifndef _PPBOX_AVFORMAT_MP4_BOX_MP4_BOX_DEFINE_H_
#define _PPBOX_AVFORMAT_MP4_BOX_MP4_BOX_DEFINE_H_

namespace ppbox
{
    namespace avformat
    {

        class Mp4BoxBase;

        struct Mp4BoxDefine
        {
            typedef void (* constuct_t)(
                Mp4BoxBase *);
            typedef void (* copy_t)(
                Mp4BoxBase *, 
                Mp4BoxBase const *);
            typedef void (* load_t)(
                Mp4BoxBase *, 
                void *);
            typedef void (* save_t)(
                Mp4BoxBase const *, 
                void *);
            typedef void (* destroy_t)(
                Mp4BoxBase *);

            Mp4BoxDefine()
                : construct(NULL)
                , copy(NULL)
                , load(NULL)
                , save(NULL)
                , destroy(NULL)
            {
            }

            constuct_t construct;
            copy_t copy;
            load_t load;
            save_t save;
            destroy_t destroy;
        };

    } // namespace avformat
} // namespace ppbox

#endif // _PPBOX_AVFORMAT_MP4_BOX_MP4_BOX_DEFINE_H_
