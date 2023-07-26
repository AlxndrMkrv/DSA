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

<<<<<<< HEAD
class Performance:
    def __init__(self, max_value: int = 1000, steps_number: int = 1000):
        recursive = lambda x: int(x) if x < 1 else int(recursive(x)*0.95)
        self.x = [max_value := int(max_value * 0.95)
                  for _ in range(steps_number) if max_value > 1][::-1]

    @staticmethod
    def __init_list(TList, n: int):
        return TList([i for i in range(n)])

    def measure_append_time(self, TList, replay: int = 10):
        return [timeit(f"[l.append(0) for i in range({x})]", number=replay,
                       globals={'l': TList()})
                for x in self.x]

    def measure_get_time(self, TList, replay: int = 10):
        return [timeit(f"l[{int(x/2)}]", number=replay,
                       globals={'l': self.__init_list(TList, x)})
                for x in self.x]

    def measure_size_time(self, TList, replay: int = 10):
        return [timeit("len(l)", number=replay,
                       globals={'l': self.__init_list(TList, x)})
                for x in self.x]

    def measure_insert_time(self, TList, replay: int = 10):
        return [timeit(f"l.insert({int(x/2)}, 0)", number=replay,
                       globals={'l': self.__init_list(TList, x)})
                for x in self.x]

    def measure_pop_time(self, TList, replay: int = 10):
        return [timeit(f"l.pop({int(x/2)}); l.insert({int(x/2)}, 0)", number=replay,
                       globals={'l': self.__init_list(TList, x)})
                for x in self.x]

    def measure_init_time(self, TList, replay: int = 10):
        return [timeit(f"L(range({x}))", number=replay,
                       globals={'L': TList})
                for x in self.x]

    def worker(self, kw: dict):
        #dict.update(kw, {"values": kw["method"](kw["TList"])})
        print(f"worker({kw})")
        return kw

    def __add_plot(self, kw: dict):
        print(f"__add_plot({kw})")
        #kw["ax"].plot(self.x, kw["values"], label=kw["label"])

=======
class Performance (Basic):
>>>>>>> d340b2d (Added array implementation and performance. Tests needed. Made some minor updates)
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

