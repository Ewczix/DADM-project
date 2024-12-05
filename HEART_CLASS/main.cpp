#include "heart_class.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QVector<double> ecgSignal = {0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.18686131, 0.15101379, 0.09618816, 0.04136253, -0.01346310, -0.06828873, -0.12311436, -0.17793998, 0.02116788, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, -0.26350365, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, 0.11313869, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 0.81021898, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, 0.00288129, 0.02235882, 0.04183634, 0.06131387, 0.08079139, 0.10026892, 0.11974645, 0.13922397, 0.15870150, 0.17817902, 0.19765655, 0.21713408, 0.23661160, 0.25608913, 0.27556665, 0.29504418, 0.30000000, 0.27572032, 0.24975029, 0.22378025, 0.19781022, 0.17184018, 0.14587015, 0.11990012, 0.09393008, 0.06796005, 0.04199001, 0.01601998, -0.00995006, -0.03592009, -0.06189013, -0.08786016, -0.03430657, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.11705426, 0.17863910, 0.12041344, 0.06218777, 0.00396210, -0.05426357, -0.11248923, -0.17071490, -0.00465116, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, 0.31395349, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 0.74806202, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.02558140, 0.01933905, 0.04002448, 0.06070991, 0.08139535, 0.10208078, 0.12276622, 0.14345165, 0.16413709, 0.18482252, 0.20550796, 0.22619339, 0.24687882, 0.26756426, 0.28824969, 0.30000000, 0.28155855, 0.25397797, 0.22639739, 0.19881681, 0.17123623, 0.14365565, 0.11607507, 0.08849449, 0.06091391, 0.03333333, 0.00575275, -0.02182783, -0.04940840, -0.07698898, -0.07829457, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10941704, 0.18520179, 0.17289487, 0.13921276, 0.10553064, 0.07184853, 0.03816642, 0.00448430, -0.02919781, -0.06287992, -0.09656203, -0.13024415, -0.16392626, -0.19760837, 0.01121076, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, -0.23363229, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, 0.56950673, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 0.45784753, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.12780269, 0.00523956, 0.01720557, 0.02917158, 0.04113760, 0.05310361, 0.06506962, 0.07703564, 0.08900165, 0.10096767, 0.11293368, 0.12489969, 0.13686571, 0.14883172, 0.16079773, 0.17276375, 0.18472976, 0.19669578, 0.20866179, 0.22062780, 0.23259382, 0.24455983, 0.25652584, 0.26849186, 0.28045787, 0.29242388, 0.30000000, 0.29924475, 0.28329006, 0.26733538, 0.25138069, 0.23542601, 0.21947132, 0.20351664, 0.18756195, 0.17160727, 0.15565258, 0.13969790, 0.12374321, 0.10778853, 0.09183384, 0.07587916, 0.05992447, 0.04396979, 0.02801511, 0.01206042, -0.00389426, -0.01984895, -0.03580363, -0.05175832, -0.06771300, -0.08366769, -0.09962237, -0.02600897, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.40000000, 0.31111111, 0.22222222, 0.13333333, 0.04444444, -0.04444444, -0.13333333, -0.22222222, -0.31111111, -0.40000000, 0.30000000, 0.30000000, 0.30000000, 0.30000000, 0.30000000, 0.30000000, 0.30000000, 0.30000000, 0.30000000, 0.30000000, -0.70000000, -0.70000000, -0.70000000, -0.70000000, -0.70000000, -0.70000000, -0.70000000, -0.70000000, -0.70000000, -0.70000000, -0.70000000, -0.70000000, -0.70000000, -0.70000000, -0.70000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, 0.00000000, 0.03157895, 0.06315789, 0.09473684, 0.12631579, 0.15789474, 0.18947368, 0.22105263, 0.25263158, 0.28421053, 0.31578947, 0.34736842, 0.37894737, 0.41052632, 0.44210526, 0.47368421, 0.50526316, 0.53684211, 0.56842105, 0.60000000, 0.60000000, 0.55789474, 0.51578947, 0.47368421, 0.43157895, 0.38947368, 0.34736842, 0.30526316, 0.26315789, 0.22105263, 0.17894737, 0.13684211, 0.09473684, 0.05263158, 0.01052632, -0.03157895, -0.07368421, -0.11578947, -0.15789474, -0.20000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 0.36710526, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, 0.00083102, 0.01838643, 0.03594183, 0.05349723, 0.07105263, 0.08860803, 0.10616343, 0.12371884, 0.14127424, 0.15882964, 0.17638504, 0.19394044, 0.21149584, 0.22905125, 0.24660665, 0.26416205, 0.28171745, 0.29927285, 0.29861496, 0.27520776, 0.25180055, 0.22839335, 0.20498615, 0.18157895, 0.15817175, 0.13476454, 0.11135734, 0.08795014, 0.06454294, 0.04113573, 0.01772853, -0.00567867, -0.02908587, -0.05249307, -0.07590028, -0.09930748, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.15521739, 0.18724638, 0.15458937, 0.12193237, 0.08927536, 0.05661836, 0.02396135, -0.00869565, -0.04135266, -0.07400966, -0.10666667, -0.13932367, -0.17198068, -0.16869565, 0.05173913, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, -0.13478261, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.36956522, 0.73260870, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 0.14086957, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.19304348, 0.00144165, 0.01304348, 0.02464531, 0.03624714, 0.04784897, 0.05945080, 0.07105263, 0.08265446, 0.09425629, 0.10585812, 0.11745995, 0.12906178, 0.14066362, 0.15226545, 0.16386728, 0.17546911, 0.18707094, 0.19867277, 0.21027460, 0.22187643, 0.23347826, 0.24508009, 0.25668192, 0.26828375, 0.27988558, 0.29148741, 0.30000000, 0.30000000, 0.28599542, 0.27052632, 0.25505721, 0.23958810, 0.22411899, 0.20864989, 0.19318078, 0.17771167, 0.16224256, 0.14677346, 0.13130435, 0.11583524, 0.10036613, 0.08489703, 0.06942792, 0.05395881, 0.03848970, 0.02302059, 0.00755149, -0.00791762, -0.02338673, -0.03885584, -0.05432494, -0.06979405, -0.08526316, -0.09652174, -0.02304348, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.40000000, 0.31111111, 0.22222222, 0.13333333, 0.04444444, -0.04444444, -0.13333333, -0.22222222, -0.31111111, -0.40000000, 0.30000000, 0.30000000, 0.30000000, 0.30000000, 0.30000000, 0.30000000, 0.30000000, 0.30000000, 0.30000000, 0.30000000, -0.70000000, -0.70000000, -0.70000000, -0.70000000, -0.70000000, -0.70000000, -0.70000000, -0.70000000, -0.70000000, -0.70000000, -0.70000000, -0.70000000, -0.70000000, -0.70000000, -0.70000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, 0.00000000, 0.03157895, 0.06315789, 0.09473684, 0.12631579, 0.15789474, 0.18947368, 0.22105263, 0.25263158, 0.28421053, 0.31578947, 0.34736842, 0.37894737, 0.41052632, 0.44210526, 0.47368421, 0.50526316, 0.53684211, 0.56842105, 0.60000000, 0.60000000, 0.55789474, 0.51578947, 0.47368421, 0.43157895, 0.38947368, 0.34736842, 0.30526316, 0.26315789, 0.22105263, 0.17894737, 0.13684211, 0.09473684, 0.05263158, 0.01052632, -0.03157895, -0.07368421, -0.11578947, -0.15789474, -0.20000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.15062500, 0.17500000, 0.12805556, 0.08111111, 0.03416667, -0.01277778, -0.05972222, -0.10666667, -0.15361111, -0.19625000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, -0.24500000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, 0.04375000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 0.43937500, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.23437500, 0.00434211, 0.02101974, 0.03769737, 0.05437500, 0.07105263, 0.08773026, 0.10440789, 0.12108553, 0.13776316, 0.15444079, 0.17111842, 0.18779605, 0.20447368, 0.22115132, 0.23782895, 0.25450658, 0.27118421, 0.28786184, 0.30000000, 0.29276316, 0.27052632, 0.24828947, 0.22605263, 0.20381579, 0.18157895, 0.15934211, 0.13710526, 0.11486842, 0.09263158, 0.07039474, 0.04815789, 0.02592105, 0.00368421, -0.01855263, -0.04078947, -0.06302632, -0.08526316, -0.06437500, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.13888889, 0.16754850, 0.10793651, 0.04832451, -0.01128748, -0.07089947, -0.13051146, -0.19012346, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, -0.20476190, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.10714286, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 0.50476190, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.25952381, 0.00751880, 0.02869674, 0.04987469, 0.07105263, 0.09223058, 0.11340852, 0.13458647, 0.15576441, 0.17694236, 0.19812030, 0.21929825, 0.24047619, 0.26165414, 0.28283208, 0.30000000, 0.28746867, 0.25923141, 0.23099415, 0.20275689, 0.17451963, 0.14628237, 0.11804511, 0.08980785, 0.06157059, 0.03333333, 0.00509607, -0.02314119, -0.05137845, -0.07961571, -0.06269841, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.13888889, 0.16754850, 0.10793651, 0.04832451, -0.01128748, -0.07089947, -0.13051146, -0.19012346, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, -0.20476190, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.10714286, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 0.50476190, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.25952381, 0.00751880, 0.02869674, 0.04987469, 0.07105263, 0.09223058, 0.11340852, 0.13458647, 0.15576441, 0.17694236, 0.19812030, 0.21929825, 0.24047619, 0.26165414, 0.28283208, 0.30000000, 0.28746867, 0.25923141, 0.23099415, 0.20275689, 0.17451963, 0.14628237, 0.11804511, 0.08980785, 0.06157059, 0.03333333, 0.00509607, -0.02314119, -0.05137845, -0.07961571, -0.06269841, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.13888889, 0.19238683, 0.15761317, 0.12283951, 0.08806584, 0.05329218, 0.01851852, -0.01625514, -0.05102881, -0.08580247, -0.12057613, -0.15534979, -0.19012346, -0.03194444, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, -0.33888889, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, 0.39583333, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 0.79537037, -0.22175926, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.29305556, -0.05833333, 0.00928363, 0.02163743, 0.03399123, 0.04634503, 0.05869883, 0.07105263, 0.08340643, 0.09576023, 0.10811404, 0.12046784, 0.13282164, 0.14517544, 0.15752924, 0.16988304, 0.18223684, 0.19459064, 0.20694444, 0.21929825, 0.23165205, 0.24400585, 0.25635965, 0.26871345, 0.28106725, 0.29342105, 0.30000000, 0.29688109, 0.28040936, 0.26393762, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.40000000, 0.31111111, 0.22222222, 0.13333333, 0.04444444, -0.04444444, -0.13333333, -0.22222222, -0.31111111, -0.40000000, 0.30000000, 0.30000000, 0.30000000, 0.30000000, 0.30000000, 0.30000000, 0.30000000, 0.30000000, 0.30000000, 0.30000000, -0.70000000, -0.70000000, -0.70000000, -0.70000000, -0.70000000, -0.70000000, -0.70000000, -0.70000000, -0.70000000, -0.70000000, -0.70000000, -0.70000000, -0.70000000, -0.70000000, -0.70000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, 2.00000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, 0.00000000, 0.03157895, 0.06315789, 0.09473684, 0.12631579, 0.15789474, 0.18947368, 0.22105263, 0.25263158, 0.28421053, 0.31578947, 0.34736842, 0.37894737, 0.41052632, 0.44210526, 0.47368421, 0.50526316, 0.53684211, 0.56842105, 0.60000000, 0.60000000, 0.55789474, 0.51578947, 0.47368421, 0.43157895, 0.38947368, 0.34736842, 0.30526316, 0.26315789, 0.22105263, 0.17894737, 0.13684211, 0.09473684, 0.05263158, 0.01052632, -0.03157895, -0.07368421, -0.11578947, -0.15789474, -0.20000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.01435789, -0.01410526, -0.04256842, -0.07103158, -0.09949474, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.13888889, 0.16754850, 0.10793651, 0.04832451, -0.01128748, -0.07089947, -0.13051146, -0.19012346, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, -0.20476190, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.10714286, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 0.50476190, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.25952381, 0.00751880, 0.02869674, 0.04987469, 0.07105263, 0.09223058, 0.11340852, 0.13458647, 0.15576441, 0.17694236, 0.19812030, 0.21929825, 0.24047619, 0.26165414, 0.28283208, 0.30000000, 0.28746867, 0.25923141, 0.23099415, 0.20275689, 0.17451963, 0.14628237, 0.11804511, 0.08980785, 0.06157059, 0.03333333, 0.00509607, -0.02314119, -0.05137845, -0.07961571, -0.06269841, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10535714, 0.19735450, 0.15264550, 0.10793651, 0.06322751, 0.01851852, -0.02619048, -0.07089947, -0.11560847, -0.16031746, -0.16607143, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, -0.00357143, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.10714286, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 0.50476190, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.15892857, 0.00751880, 0.02340226, 0.03928571, 0.05516917, 0.07105263, 0.08693609, 0.10281955, 0.11870301, 0.13458647, 0.15046992, 0.16635338, 0.18223684, 0.19812030, 0.21400376, 0.22988722, 0.24577068, 0.26165414, 0.27753759, 0.29342105, 0.30000000, 0.28746867, 0.26629073, 0.24511278, 0.22393484, 0.20275689, 0.18157895, 0.16040100, 0.13922306, 0.11804511, 0.09686717, 0.07568922, 0.05451128, 0.03333333, 0.01215539, -0.00902256, -0.03020050, -0.05137845, -0.07255639, -0.09373434, -0.02916667, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.17971014, 0.15458937, 0.10016103, 0.04573269, -0.00869565, -0.06312399, -0.11755233, -0.17198068, -0.02173913, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, -0.13478261, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.36956522, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, -0.17753623, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.11956522, 0.01304348, 0.03237986, 0.05171625, 0.07105263, 0.09038902, 0.10972540, 0.12906178, 0.14839817, 0.16773455, 0.18707094, 0.20640732, 0.22574371, 0.24508009, 0.26441648, 0.28375286, 0.30000000, 0.29115179, 0.26536995, 0.23958810, 0.21380625, 0.18802441, 0.16224256, 0.13646072, 0.11067887, 0.08489703, 0.05911518, 0.03333333, 0.00755149, -0.01823036, -0.04401220, -0.06979405, -0.09557590, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.00000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.12181818, 0.19863636, 0.16646465, 0.13232323, 0.09818182, 0.06404040, 0.02989899, -0.00424242, -0.03838384, -0.07252525, -0.10666667, -0.14080808, -0.17494949, -0.13863636, 0.09181818, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, 0.10000000, -0.01454545, -0.47545455, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, -0.50000000, 0.33181818, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 1.00000000, 0.31454545, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.30000000, -0.26};
    QApplication a(argc, argv);
    double fs = 250;
    HEART_CLASS anomalyDetection = HEART_CLASS(fs);
    HEART_CLASS::AnomalyDetectionResult anomalies = anomalyDetection.detectAnomalies(ecgSignal);
    anomalyDetection.show();
    return a.exec();
}
