#pragma once

#include <cmath>
#include <cstdio>
#include <cstring>

#include "labeldef.h"

namespace esphome {
namespace altherma_hub {

class Converter
{
public:
    double convertPress2Temp(double data)
    {
        // Approximation for R32 refrigerant.
        const double num1 =
            -2.6989493795556E-07 *
            data * data * data * data * data * data;

        const double num2 =
            4.26383417104661E-05 *
            data * data * data * data * data;

        const double num3 =
            -0.00262978346547749 *
            data * data * data * data;

        const double num4 =
            0.0805858127503585 *
            data * data * data;

        const double num5 =
            -1.31924457284073 *
            data * data;

        const double num6 =
            13.4157368435437 * data;

        const double num7 =
            -51.1813342993155;

        return num1 + num2 + num3 + num4 + num5 + num6 + num7;
    }

    void convert(LabelDef *def, unsigned char *data)
    {
        if (def == nullptr || data == nullptr)
        {
            return;
        }

        def->asString[0] = '\0';

        const int convId = def->convid;
        const int dataSize = def->dataSize;

        double dblData = NAN;

        switch (convId)
        {
        case 100:
        {
            const size_t max_length = sizeof(def->asString) - 1;
            const size_t copy_length =
                static_cast<size_t>(dataSize) < max_length
                    ? static_cast<size_t>(dataSize)
                    : max_length;

            std::memcpy(def->asString, data, copy_length);
            def->asString[copy_length] = '\0';
            return;
        }

        case 101:
            dblData = static_cast<double>(
                getSignedValue(data, dataSize, 0));
            break;

        case 102:
            dblData = static_cast<double>(
                getSignedValue(data, dataSize, 1));
            break;

        case 103:
            dblData = static_cast<double>(
                          getSignedValue(data, dataSize, 0)) /
                      256.0;
            break;

        case 104:
            dblData = static_cast<double>(
                          getSignedValue(data, dataSize, 1)) /
                      256.0;
            break;

        case 105:
            dblData = static_cast<double>(
                          getSignedValue(data, dataSize, 0)) *
                      0.1;
            break;

        case 106:
            dblData = static_cast<double>(
                          getSignedValue(data, dataSize, 1)) *
                      0.1;
            break;

        case 107:
            dblData = static_cast<double>(
                          getSignedValue(data, dataSize, 0)) *
                      0.1;

            if (dblData == -3276.8)
            {
                copyString(def->asString, "---");
                return;
            }
            break;

        case 108:
            dblData = static_cast<double>(
                          getSignedValue(data, dataSize, 1)) *
                      0.1;

            if (dblData == -3276.8)
            {
                copyString(def->asString, "---");
                return;
            }
            break;

        case 109:
            dblData = static_cast<double>(
                          getSignedValue(data, dataSize, 0)) /
                      256.0 * 2.0;
            break;

        case 110:
            dblData = static_cast<double>(
                          getSignedValue(data, dataSize, 1)) /
                      256.0 * 2.0;
            break;

        case 111:
            dblData = static_cast<double>(
                          getSignedValue(data, dataSize, 1)) *
                      0.5;
            break;

        case 112:
            dblData = static_cast<double>(
                          getSignedValue(data, dataSize, 1) - 64) *
                      0.5;
            break;

        case 113:
            dblData = static_cast<double>(
                          getSignedValue(data, dataSize, 1)) *
                      0.25;
            break;

        case 114:
        {
            if (data[0] == 0 && data[1] == 128)
            {
                copyString(def->asString, "---");
                return;
            }

            unsigned short value =
                static_cast<unsigned short>(
                    static_cast<unsigned int>(data[1]) * 256U);

            value |= static_cast<unsigned short>(data[0]);

            const bool negative = (data[1] & 128U) != 0;

            if (negative)
            {
                value = static_cast<unsigned short>(~(value - 1));
            }

            dblData =
                static_cast<double>((value & 65280U) / 256U);

            dblData +=
                static_cast<double>(value & 255U) / 256.0;

            dblData *= 10.0;

            if (negative)
            {
                dblData *= -1.0;
            }

            break;
        }

        case 115:
            dblData = static_cast<double>(
                          getSignedValue(data, dataSize, 0)) /
                      2560.0;
            break;

        case 116:
            dblData = static_cast<double>(
                          getSignedValue(data, dataSize, 1)) /
                      2560.0;
            break;

        case 117:
            dblData = static_cast<double>(
                          getSignedValue(data, dataSize, 0)) *
                      0.01;
            break;

        case 118:
            dblData = static_cast<double>(
                          getSignedValue(data, dataSize, 1)) *
                      0.01;
            break;

        case 119:
        {
            if (data[0] == 0 && data[1] == 128)
            {
                copyString(def->asString, "---");
                return;
            }

            unsigned short value =
                static_cast<unsigned short>(
                    static_cast<unsigned int>(data[1]) * 256U);

            value |= static_cast<unsigned short>(data[0] & 127U);

            dblData =
                static_cast<double>((value & 65280U) / 256U);

            dblData +=
                static_cast<double>(value & 255U) / 256.0;

            break;
        }

        case 151:
            dblData = static_cast<double>(
                getUnsignedValue(data, dataSize, 0));
            break;

        case 152:
            dblData = static_cast<double>(
                getUnsignedValue(data, dataSize, 1));
            break;

        case 153:
            dblData = static_cast<double>(
                          getUnsignedValue(data, dataSize, 0)) /
                      256.0;
            break;

        case 154:
            dblData = static_cast<double>(
                          getUnsignedValue(data, dataSize, 1)) /
                      256.0;
            break;

        case 155:
            dblData = static_cast<double>(
                          getUnsignedValue(data, dataSize, 0)) *
                      0.1;
            break;

        case 156:
            dblData = static_cast<double>(
                          getUnsignedValue(data, dataSize, 1)) *
                      0.1;
            break;

        case 157:
            dblData = static_cast<double>(
                          getUnsignedValue(data, dataSize, 0)) /
                      256.0 * 2.0;
            break;

        case 158:
            dblData = static_cast<double>(
                          getUnsignedValue(data, dataSize, 1)) /
                      256.0 * 2.0;
            break;

        case 161:
            dblData = static_cast<double>(
                          getUnsignedValue(data, dataSize, 1)) *
                      0.5;
            break;

        case 162:
            dblData = static_cast<double>(
                          getUnsignedValue(data, dataSize, 1) - 64) *
                      0.5;
            break;

        case 163:
            dblData = static_cast<double>(
                          getUnsignedValue(data, dataSize, 1)) *
                      0.25;
            break;

        case 164:
            dblData = static_cast<double>(
                          getUnsignedValue(data, dataSize, 1)) *
                      5.0;
            break;

        case 165:
            dblData = static_cast<double>(
                getUnsignedValue(data, dataSize, 0) & 16383U);
            break;

        case 200:
            convertTable200(data, def->asString);
            return;

        case 203:
            convertTable203(data, def->asString);
            return;

        case 204:
            convertTable204(data, def->asString);
            return;

        case 211:
            if (data[0] == 0)
            {
                copyString(def->asString, "OFF");
                return;
            }

            dblData = static_cast<double>(data[0]);
            break;

        case 215:
        case 216:
        {
            const int high = data[0] >> 4;
            const int low = data[0] & 15;

            std::snprintf(
                def->asString,
                sizeof(def->asString),
                "{0:%d}{1:%d}",
                high,
                low);

            return;
        }

        case 201:
        case 217:
            convertTable217(data, def->asString);
            return;

        case 300:
        case 301:
        case 302:
        case 303:
        case 304:
        case 305:
        case 306:
        case 307:
            convertTable300(
                data,
                def->convid,
                def->asString);
            return;

        case 312:
            dblData = convertTable312(data);
            break;

        case 315:
            convertTable315(data, def->asString);
            return;

        case 316:
            convertTable316(data, def->asString);
            return;

        // Pressure to temperature conversion.
        case 401:
            dblData = static_cast<double>(
                getSignedValue(data, dataSize, 0));

            dblData = convertPress2Temp(dblData);
            break;

        case 402:
            dblData = static_cast<double>(
                getSignedValue(data, dataSize, 1));

            dblData = convertPress2Temp(dblData);
            break;

        case 403:
            dblData = static_cast<double>(
                          getSignedValue(data, dataSize, 0)) /
                      256.0;

            dblData = convertPress2Temp(dblData);
            break;

        case 404:
            dblData = static_cast<double>(
                          getSignedValue(data, dataSize, 1)) /
                      256.0;

            dblData = convertPress2Temp(dblData);
            break;

        case 405:
            dblData = static_cast<double>(
                          getSignedValue(data, dataSize, 0)) *
                      0.1;

            dblData = convertPress2Temp(dblData);
            break;

        case 406:
            dblData = static_cast<double>(
                          getSignedValue(data, dataSize, 1)) *
                      0.1;

            dblData = convertPress2Temp(dblData);
            break;

        default:
            std::snprintf(
                def->asString,
                sizeof(def->asString),
                "Conv %d not avail.",
                convId);

            return;
        }

        if (!std::isnan(dblData))
        {
            std::snprintf(
                def->asString,
                sizeof(def->asString),
                "%g",
                dblData);
        }
    }

private:
    static void copyString(char *destination, const char *source)
    {
        if (destination == nullptr || source == nullptr)
        {
            return;
        }

        std::snprintf(destination, 30, "%s", source);
    }

    void convertTable300(
        unsigned char *data,
        int tableID,
        char *ret)
    {
        unsigned char bit =
            static_cast<unsigned char>(
                1U << static_cast<unsigned int>(tableID % 10));

        if ((data[0] & bit) != 0)
        {
            copyString(ret, "ON");
        }
        else
        {
            copyString(ret, "OFF");
        }
    }

    void convertTable203(
        unsigned char *data,
        char *ret)
    {
        switch (data[0])
        {
        case 0:
            copyString(ret, "Normal");
            break;

        case 1:
            copyString(ret, "Error");
            break;

        case 2:
            copyString(ret, "Warning");
            break;

        case 3:
            copyString(ret, "Caution");
            break;

        default:
            copyString(ret, "-");
            break;
        }
    }

    void convertTable204(
        unsigned char *data,
        char *ret)
    {
        static const char first[] =
            " ACEHFJLPU987654";

        static const char second[] =
            "0123456789AHCJEF";

        const unsigned int high =
            static_cast<unsigned int>((data[0] >> 4) & 15U);

        const unsigned int low =
            static_cast<unsigned int>(data[0] & 15U);

        ret[0] = first[high];
        ret[1] = second[low];
        ret[2] = '\0';
    }

    double convertTable312(unsigned char *data)
    {
        double value =
            static_cast<double>(
                static_cast<unsigned char>(
                    (data[0] >> 4) & 7U) +
                static_cast<unsigned char>(
                    data[0] & 15U)) /
            16.0;

        if ((data[0] & 128U) != 0)
        {
            value *= -1.0;
        }

        return value;
    }

    void convertTable315(
        unsigned char *data,
        char *ret)
    {
        const unsigned char value =
            static_cast<unsigned char>(
                (data[0] & 240U) >> 4);

        switch (value)
        {
        case 0:
            copyString(ret, "Stop");
            break;

        case 1:
            copyString(ret, "Heating");
            break;

        case 2:
            copyString(ret, "Cooling");
            break;

        case 3:
            copyString(ret, "??");
            break;

        case 4:
            copyString(ret, "DHW");
            break;

        case 5:
            copyString(ret, "Heating + DHW");
            break;

        case 6:
            copyString(ret, "Cooling + DHW");
            break;

        default:
            copyString(ret, "-");
            break;
        }
    }

    void convertTable316(
        unsigned char *data,
        char *ret)
    {
        const unsigned char value =
            static_cast<unsigned char>(
                (data[0] & 240U) >> 4);

        switch (value)
        {
        case 0:
            copyString(ret, "H/P only");
            break;

        case 1:
            copyString(ret, "Hybrid");
            break;

        case 2:
            copyString(ret, "Boiler only");
            break;

        default:
            copyString(ret, "Unknown");
            break;
        }
    }

    void convertTable200(
        unsigned char *data,
        char *ret)
    {
        copyString(
            ret,
            data[0] == 0 ? "OFF" : "ON");
    }

    void convertTable217(
        unsigned char *data,
        char *ret)
    {
        static const char *const values[] = {
            "Fan Only",
            "Heating",
            "Cooling",
            "Auto",
            "Ventilation",
            "Auto Cool",
            "Auto Heat",
            "Dry",
            "Aux.",
            "Cooling Storage",
            "Heating Storage",
            "UseStrdThrm(cl)1",
            "UseStrdThrm(cl)2",
            "UseStrdThrm(cl)3",
            "UseStrdThrm(cl)4",
            "UseStrdThrm(ht)1",
            "UseStrdThrm(ht)2",
            "UseStrdThrm(ht)3",
            "UseStrdThrm(ht)4"
        };

        constexpr size_t value_count =
            sizeof(values) / sizeof(values[0]);

        const size_t index =
            static_cast<size_t>(data[0]);

        if (index >= value_count)
        {
            copyString(ret, "Unknown");
            return;
        }

        copyString(ret, values[index]);
    }

    unsigned short getUnsignedValue(
        unsigned char *data,
        int dataSize,
        int cnvflg)
    {
        if (data == nullptr || dataSize <= 0)
        {
            return 0;
        }

        if (dataSize == 1)
        {
            return static_cast<unsigned short>(data[0]);
        }

        if (cnvflg == 0)
        {
            return static_cast<unsigned short>(
                (static_cast<unsigned short>(data[1]) << 8) |
                static_cast<unsigned short>(data[0]));
        }

        return static_cast<unsigned short>(
            (static_cast<unsigned short>(data[0]) << 8) |
            static_cast<unsigned short>(data[1]));
    }

    short getSignedValue(
        unsigned char *data,
        int dataSize,
        int cnvflg)
    {
        const unsigned short unsignedValue =
            getUnsignedValue(data, dataSize, cnvflg);

        return static_cast<short>(unsignedValue);
    }
};

}  // namespace altherma_hub
}  // namespace esphome
