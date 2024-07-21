#include "Sprite.h"
#include "Core/Window.h"

Sprite::Sprite(wz::Node* node, int alpha)
{
    // 如果节点类型为UOL，则将其转换为UOL类型
    if (node->type == wz::Type::UOL)
    {
        node = dynamic_cast<wz::Property<wz::WzUOL>*>(node)->get_uol();
    }

    // 将节点转换为Canvas类型
    auto canvas = dynamic_cast<wz::Property<wz::WzCanvas>*>(node);
    // 获取Canvas的高度和宽度
    height = canvas->get().height;
    width  = canvas->get().width;

    // 获取Canvas的原始数据
    auto raw_data = canvas->get_raw_data();
    // 获取Canvas的格式
    auto format = canvas->get().format + canvas->get().format2;

    // 获取Canvas的origin属性
    auto o = dynamic_cast<wz::Property<wz::WzVec2D>*>(canvas->get_child(u"origin"));

    // 初始化origin的x和y坐标
    auto ox = 0;
    auto oy = 0;

    // 如果origin属性存在，则获取其x和y坐标
    if (o != nullptr)
    {
        ox = o->get().x;
        oy = o->get().y;
    }
    // 设置origin属性
    origin = {ox, oy};

    // 设置默认的delay为100
    delay = 100;

    // 如果节点存在delay属性，则获取其值
    if (node->get_child(u"delay") != nullptr)
    {
        // 如果delay属性为字符串类型，则将其转换为整数
        if (node->get_child(u"delay")->type == wz::Type::String)
        {
            auto delay_str = dynamic_cast<wz::Property<wz::wzstring>*>(node->get_child(u"delay"))->get();
            delay          = std::stoi(std::string {delay_str.begin(), delay_str.end()});
        }
        // 如果delay属性为整数类型，则直接获取其值
        else if (node->get_child(u"delay")->type == wz::Type::Int)
        {
            delay = dynamic_cast<wz::Property<int>*>(node->get_child(u"delay"))->get();
        }
    }

    // 如果Canvas存在a0和a1属性，则获取其值
    if (canvas->get_child(u"a0") != nullptr && canvas->get_child(u"a1") != nullptr)
    {
        // 如果a0属性为整数类型，则直接获取其值
        if (canvas->get_child(u"a0")->type == wz::Type::Int)
        {
            a0 = dynamic_cast<wz::Property<int>*>(canvas->get_child(u"a0"))->get();
        }
        // 如果a0属性为字符串类型，则将其转换为整数
        else
        {
            auto a0_str = dynamic_cast<wz::Property<wz::wzstring>*>(node->get_child(u"a0"))->get();
            a0          = std::stoi(std::string {a0_str.begin(), a0_str.end()});
        }
        // 如果a1属性为整数类型，则直接获取其值
        if (canvas->get_child(u"a1")->type == wz::Type::Int)
        {
            a1 = dynamic_cast<wz::Property<int>*>(canvas->get_child(u"a1"))->get();
        }
        // 如果a1属性为字符串类型，则将其转换为整数
        else
        {
            auto a1_str = dynamic_cast<wz::Property<wz::wzstring>*>(node->get_child(u"a1"))->get();
            a1          = std::stoi(std::string {a1_str.begin(), a1_str.end()});
        }
    }
    // 将a0和a1乘以alpha值
    a0 = a0 * ((float)alpha / 255);
    a1 = a1 * ((float)alpha / 255);

    // 如果Canvas存在z属性，则获取其值
    if (canvas->get_child(u"z") != nullptr)
    {
        // 如果z属性为整数类型，则直接获取其值
        if (canvas->get_child(u"z")->type == wz::Type::Int)
        {
            z = dynamic_cast<wz::Property<int>*>(canvas->get_child(u"z"))->get();
        }
        // 如果z属性为字符串类型，则直接获取其值
        else if (canvas->get_child(u"z")->type == wz::Type::String)
        {
            z = dynamic_cast<wz::Property<wz::wzstring>*>(canvas->get_child(u"z"))->get();
        }
    }

    // 图片原始数据,部分格式需要转换
    std::vector<uint8_t> pixel;

    // 根据格式创建纹理
    switch (format)
    {
        case 1: { // ARGB4444
            pixel   = raw_data;
            format  = SDL_PIXELFORMAT_ARGB4444;
            texture = SDL_CreateTexture(Window::get_renderer(), format, SDL_TEXTUREACCESS_STATIC, width, height);
            SDL_UpdateTexture(texture, nullptr, pixel.data(), width * sizeof(Uint16));
            SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
            break;
        }
        case 2: { // ARGB8888
            pixel   = raw_data;
            format  = SDL_PIXELFORMAT_ARGB8888;
            texture = SDL_CreateTexture(Window::get_renderer(), format, SDL_TEXTUREACCESS_STATIC, width, height);
            SDL_UpdateTexture(texture, nullptr, pixel.data(), width * sizeof(Uint32));
            SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
            break;
        }
        case 517: // rgb565压缩缩略图
        {
            pixel.resize(width * height * 2, 0);
            int lineIndex = 0;
            for (int j0 = 0, j1 = height / 16; j0 < j1; j0++)
            {
                int dstIndex = lineIndex;
                for (int i0 = 0, i1 = width / 16; i0 < i1; i0++)
                {
                    int           idx = (i0 + j0 * i1) * 2;
                    unsigned char b0  = raw_data[idx];
                    unsigned char b1  = raw_data[idx + 1];
                    for (int k = 0; k < 16; k++)
                    {
                        pixel[dstIndex++] = b0;
                        pixel[dstIndex++] = b1;
                    }
                }
                for (int k = 1; k < 16; k++)
                {
                    for (int m = 0; m < width * 2; m++)
                    {
                        pixel[dstIndex + m] = pixel[lineIndex + m];
                    }
                    dstIndex += width * 2;
                }
                lineIndex += width * 32;
            }

            format  = SDL_PIXELFORMAT_RGB565;
            texture = SDL_CreateTexture(Window::get_renderer(), format, SDL_TEXTUREACCESS_STATIC, width, height);
            SDL_UpdateTexture(texture, nullptr, pixel.data(), width * sizeof(Uint16));
            break;
        }
        default: {
            break;
        }
    }
}

Sprite::Sprite(wz::Node* node, int width, int height, uint8_t type)
{
    // 根据type的值，选择不同的处理方式
    switch (type)
    {
        case NameTag: {
            // 动态类型转换，获取w、e、c属性
            auto w = dynamic_cast<wz::Property<wz::WzCanvas>*>(node->find_from_path(u"w"));
            auto e = dynamic_cast<wz::Property<wz::WzCanvas>*>(node->find_from_path(u"e"));
            auto c = dynamic_cast<wz::Property<wz::WzCanvas>*>(node->find_from_path(u"c"));

            // 计算宽度
            width = width + w->get().width + e->get().width;
            // 计算高度
            height = std::max(height, w->get().height);
            // 创建纹理
            texture = SDL_CreateTexture(
                Window::get_renderer(), SDL_PIXELFORMAT_ARGB4444, SDL_TEXTUREACCESS_STATIC, width, height);

            // 更新纹理
            SDL_Rect rect {0, 0, w->get().width, w->get().height};
            SDL_UpdateTexture(texture, &rect, w->get_raw_data().data(), w->get().width * sizeof(Uint16));

            // 更新纹理
            for (int i = 0; i <= (width - w->get().width - e->get().width) / c->get().width; i++)
            {
                rect = {w->get().width + i * c->get().width, 0, c->get().width, c->get().height};
                SDL_UpdateTexture(texture, &rect, c->get_raw_data().data(), c->get().width * sizeof(Uint16));
            }
            // 更新纹理
            rect = {width - e->get().width, 0, e->get().width, e->get().height};
            SDL_UpdateTexture(texture, &rect, e->get_raw_data().data(), e->get().width * sizeof(Uint16));
            // 设置纹理混合模式
            SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
            // 设置宽度
            this->width = width;
            // 设置高度
            this->height = height;
            break;
        }
        case ChatBallon: { // 如果是聊天气泡
            // 动态类型转换，获取c、e、ne、n、nw、w、sw、s、se、arrow属性
            auto c     = dynamic_cast<wz::Property<wz::WzCanvas>*>(node->find_from_path(u"c"));     // 中间
            auto e     = dynamic_cast<wz::Property<wz::WzCanvas>*>(node->find_from_path(u"e"));     // 右边
            auto ne    = dynamic_cast<wz::Property<wz::WzCanvas>*>(node->find_from_path(u"ne"));    // 右下
            auto n     = dynamic_cast<wz::Property<wz::WzCanvas>*>(node->find_from_path(u"n"));     // 上
            auto nw    = dynamic_cast<wz::Property<wz::WzCanvas>*>(node->find_from_path(u"nw"));    // 左上
            auto w     = dynamic_cast<wz::Property<wz::WzCanvas>*>(node->find_from_path(u"w"));     // 左
            auto sw    = dynamic_cast<wz::Property<wz::WzCanvas>*>(node->find_from_path(u"sw"));    // 左下
            auto s     = dynamic_cast<wz::Property<wz::WzCanvas>*>(node->find_from_path(u"s"));     // 下
            auto se    = dynamic_cast<wz::Property<wz::WzCanvas>*>(node->find_from_path(u"se"));    // 右下
            auto arrow = dynamic_cast<wz::Property<wz::WzCanvas>*>(node->find_from_path(u"arrow")); // 箭头

            // 计算宽度
            width += nw->get().width + ne->get().width;

            // 计算高度
            height += nw->get().height + sw->get().height;
            // 计算行数
            auto line = std::ceil((float)height / (float)c->get().height);
            // 计算高度
            height = line * c->get().height;

            // 创建纹理
            texture = SDL_CreateTexture(
                Window::get_renderer(), SDL_PIXELFORMAT_ARGB4444, SDL_TEXTUREACCESS_STATIC, width, height + 8);
            SDL_Rect rect;

            // 更新纹理
            for (int x = 0; x <= width; x += c->get().width)
            {
                for (int y = 0; y <= height - c->get().height; y += c->get().height)
                {
                    rect = {x, y, c->get().width, c->get().height};
                    SDL_UpdateTexture(texture, &rect, c->get_raw_data().data(), c->get().width * sizeof(Uint16));
                }
            }

            // 更新纹理
            for (int i = nw->get().height; i <= height - w->get().height; i += w->get().height)
            {
                rect = {0, i, w->get().width, w->get().height};
                SDL_UpdateTexture(texture, &rect, w->get_raw_data().data(), w->get().width * sizeof(Uint16));

                rect = {width - e->get().width, i, e->get().width, e->get().height};
                SDL_UpdateTexture(texture, &rect, e->get_raw_data().data(), e->get().width * sizeof(Uint16));
            }

            // 更新纹理
            for (int i = 0; i <= width; i += n->get().width)
            {
                rect = {i, 0, n->get().width, n->get().height};
                SDL_UpdateTexture(texture, &rect, n->get_raw_data().data(), n->get().width * sizeof(Uint16));

                rect = {i, height - s->get().height, s->get().width, s->get().height};
                SDL_UpdateTexture(texture, &rect, s->get_raw_data().data(), s->get().width * sizeof(Uint16));
            }

            // 更新纹理
            rect = {0, nw->get().height, w->get().width, w->get().height};
            SDL_UpdateTexture(texture, &rect, w->get_raw_data().data(), w->get().width * sizeof(Uint16));

            // 更新纹理
            rect = {0, height - sw->get().height - w->get().height, w->get().width, w->get().height};
            SDL_UpdateTexture(texture, &rect, w->get_raw_data().data(), w->get().width * sizeof(Uint16));

            // 更新纹理
            rect = {width - e->get().width, nw->get().height, e->get().width, e->get().height};
            SDL_UpdateTexture(texture, &rect, e->get_raw_data().data(), e->get().width * sizeof(Uint16));

            // 更新纹理
            rect = {
                width - e->get().width, height - sw->get().height - e->get().height, e->get().width, e->get().height};
            SDL_UpdateTexture(texture, &rect, e->get_raw_data().data(), e->get().width * sizeof(Uint16));

            // 更新纹理
            rect = {0, 0, nw->get().width, nw->get().height};
            SDL_UpdateTexture(texture, &rect, nw->get_raw_data().data(), nw->get().width * sizeof(Uint16));

            // 更新纹理
            rect = {0, height - sw->get().height, sw->get().width, sw->get().height};
            SDL_UpdateTexture(texture, &rect, sw->get_raw_data().data(), sw->get().width * sizeof(Uint16));

            // 更新纹理
            rect = {width - ne->get().width, 0, ne->get().width, ne->get().height};
            SDL_UpdateTexture(texture, &rect, ne->get_raw_data().data(), ne->get().width * sizeof(Uint16));

            // 更新纹理
            rect = {width - se->get().width, height - se->get().height, se->get().width, se->get().height};
            SDL_UpdateTexture(texture, &rect, se->get_raw_data().data(), se->get().width * sizeof(Uint16));

            // 更新纹理
            rect = {width / 2 - arrow->get().width / 2,
                    height - arrow->get().height + 8,
                    arrow->get().width - 1,
                    arrow->get().height};
            SDL_UpdateTexture(texture, &rect, arrow->get_raw_data().data() + 2, arrow->get().width * sizeof(Uint16));

            // 设置纹理混合模式
            SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
            // 设置宽度
            this->width  = width;
            // 设置高度
            this->height = height;
            break;
        }
        default:
            break;
    }
}

Sprite::Sprite(SDL_Texture* texture, int w, int h) : texture(texture), width(w), height(h) {}

Sprite::~Sprite()
{
    SDL_DestroyTexture(texture);
}
