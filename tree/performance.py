from timeit import timeit
import matplotlib.pyplot as plt
import sys
import os

sys.path.append(os.curdir)

from basic_performance import Base
import tree_nb
import tree_pb

# make dict compatible with tree node
class MyDict (dict):
    def __init__(self, value: object, children: int = 0):
        dict.__init__(self)

    def insert(self, value: object):
        self[value] = None;

    def remove(self, value: object):
        self.pop(value)


TestObjects = {"dict": MyDict,
               "Tree_nano": tree_nb.Tree,
               "Tree_pb11": tree_pb.Tree
}

class Performance (Base):
    def measure_init_time(self, TContainer, replay: int = 10):
        return [timeit("TContainer(1, 1)", number=replay,
                       globals={"TContainer": TContainer, "n": n})
                for n in self.N]

    def measure_insert_time(self, TContainer, replay: int = 10):
        return [timeit("[obj.insert(i) for i in range(n)]",
                       "obj = TContainer(0)", number=replay,
                       globals={"TContainer": TContainer, "n": n})
                for n in self.N]

    def __fill_tree(self, TContainer, n: int):
        obj = TContainer(0)
        [obj.insert(i) for i in range(n)]
        return obj


    def measure_remove_time(self, TContainer, replay: int = 10):
        return [timeit("[obj.remove(i) for i in range(n)]", number=replay,
                       globals={"obj": self.__fill_tree(TContainer, n), 'n': n})
                for n in self.N]

    def measure_get_time(self, TContainer, replay: int = 10):
        return [timeit("[obj[i] for i in range(n)]", number=replay,
                       globals={"obj": self.__fill_tree(TContainer, n), 'n': n})
                for n in self.N]


    def plot(self):
        fig, ax = plt.subplots(2, 2, sharex=True)
        for i, j, title, method in [(0, 0, "Initialize", self.measure_init_time),
                                    #(0, 1, "Insert", self.measure_insert_time),
                                    #(1, 0, "Remove", self.measure_remove_time),
                                    (1, 1, "Get", self.measure_get_time)
                                    ]:
            ax[i][j].set_title(title)
            for lbl, obj in TestObjects.items():
                ax[i][j].plot(self.N, method(obj, 1), label=lbl)
            ax[i][j].legend()
            ax[i][j].grid()

        plt.tight_layout()
        plt.show()

Performance(max_value=1000).plot()

