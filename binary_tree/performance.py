import matplotlib.pyplot as plt
import sys
import os

sys.path.append(os.curdir)

from basic_performance import Base
import binary_tree_nb
import binary_tree_pb


TestObjects = {"set": set,
               "Tree_nano": binary_tree_nb.Tree,
               "Tree_pb11": binary_tree_pb.Tree
}


class Performance (Base):
    def plot(self):
        fig, ax = plt.subplots(2, 2, sharex=True)
        for i, j, title, method, rand in [(0, 0, "Linear data init",
                                           self.measure_init_time, False),
                                          (0, 1, "Random data init",
                                           self.measure_init_time, True),
                                          (1, 0, "Contains in linear",
                                           self.measure_contains_time, False),
                                          (1, 1, "Contains in random",
                                           self.measure_contains_time, True),
                                         ]:
            ax[i][j].set_title(title)
            for lbl, obj in TestObjects.items():
                ax[i][j].plot(self.N, method(obj, randomize=rand), label=lbl)
            ax[i][j].legend()
            ax[i][j].grid()

        plt.tight_layout()
        plt.show()

Performance(max_value=200).plot()

