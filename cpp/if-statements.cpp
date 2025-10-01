#include <memory>
#include <cassert>
#include <vector>
#include <array>

class Example final
{
public:
    Example();

    virtual ~Example() = default;

    Example(const Example &)            = delete;
    Example &operator=(const Example &) = delete;
    Example(Example &&)                 = delete;
    Example &operator=(Example &&)      = delete;

    void ReceiveData(uint8_t* data, size_t size) {
        if (size < 9) {
            return;
        }
        if (data[0] == 'C') {
            if (data[1] == 'o') {
                if (data[2] == 'd') {
                    if (data[3] == 'e') {
                        if (data[4] == 'F') {
                            if (data[5] == 'o') {
                                if (data[6] == 'r') {
                                    if (data[7] == 'g') {
                                        if (data[8] == 'e') {
                                            assert(!"be assertive triggered!");
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
};

Example::Example()
{
}


extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    std::unique_ptr<Example> underTest = std::make_unique<Example>();
    underTest->ReceiveData(const_cast<uint8_t*>(data), size);
    return 0;
}