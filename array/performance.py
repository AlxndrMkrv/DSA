import matplotlib.pyplot as plt
import sys
from os import curdir as os_curdir

sys.path.append(os_curdir)
from basic_performance import Base

import array_nb
import array_pb

TestObjects = {"list": list,
               "StlArray_nano": array_nb.StlArray,
               "StlArray_pb11": array_pb.StlArray
}

class Performance (Base):
    def plot(self):
        fig, ax = plt.subplots(2, 2, sharex=True)
        for i, j, title, method in [(0, 0, "Initialization", self.measure_init_time),
                                    (1, 0, "Size", self.measure_size_time),
                                    (0, 1, "Get item", self.measure_get_time)
                                    ]:
            ax[i][j].set_title(title)
            for lbl, obj in TestObjects.items():
                ax[i][j].plot(self.N, method(obj, 10), label=lbl)
            ax[i][j].legend()
            ax[i][j].grid()

        plt.tight_layout()
        plt.show()

Performance(max_value=10000).plot()


