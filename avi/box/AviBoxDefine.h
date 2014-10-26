// AviBoxDefine.h

#ifndef _PPBOX_AVFORMAT_AVI_BOX_AVI_BOX_DEFINE_H_
#define _PPBOX_AVFORMAT_AVI_BOX_AVI_BOX_DEFINE_H_

namespace ppbox
{
    namespace avformat
    {

        class AviBoxBase;

        struct AviBoxDefine
        {
            typedef void (* constuct_t)(
                AviBoxBase *);
            typedef void (* copy_t)(
                AviBoxBase *, 
                AviBoxBase const *);
            typedef void (* load_t)(
                AviBoxBase *, 
                void *);
            typedef void (* save_t)(
                AviBoxBase const *, 
                void *);
            typedef void (* destroy_t)(
                AviBoxBase *);

            AviBoxDefine()
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

#endif // _PPBOX_AVFORMAT_AVI_BOX_AVI_BOX_DEFINE_H_
