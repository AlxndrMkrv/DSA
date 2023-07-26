import matplotlib.pyplot as plt
import sys
import os

sys.path.append(os.curdir)

from basic_performance import Basic
import list_nb
import list_pb

TestObjects = {"list": list,
               "MyList_nano": list_nb.MyList,
               "MyList_pb11": list_pb.MyList,
               "StlList_nano": list_nb.StlList,
               "StlList_pb11": list_pb.StlList
}

class Performance (Basic):
    def plot(self):
        fig, ax = plt.subplots(2, 3, sharex=True)
        for i, j, title, method in [(0, 0, "Append(x)", self.measure_append_time),
                                    (1, 0, "Get middle element", self.measure_get_time),
                                    (0, 1, "Size", self.measure_size_time),
                                    (1, 1, "Insert to middle", self.measure_insert_time),
                                    (0, 2, "Pop from middle", self.measure_pop_time),
                                    (1, 2, "Initialization", self.measure_init_time)
                                    ]:
            ax[i][j].set_title(title)
            for lbl, obj in TestObjects.items():
                ax[i][j].plot(self.N, method(obj, 10), label=lbl)
            ax[i][j].legend()
            ax[i][j].grid()

        plt.tight_layout()
        plt.show()

Performance(max_value=1000).plot()

