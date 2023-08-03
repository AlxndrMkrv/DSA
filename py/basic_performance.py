from timeit import timeit
from random import shuffle, randint


class Base:
    def __init__(self, max_value: int = 1000, steps_number: int = 1000):
        step = round(max_value/steps_number)
        self.N = [int(x) for x in range(1, max_value, step if step else 1)]

    @staticmethod
    def make_sequence(n: int, randomize: bool = False):
        sequence = [i for i in range(n)]
        shuffle(sequence) if randomize else ...
        return sequence

    @staticmethod
    def __init_container(TContainer, n: int, randomize: bool = False):
        return TContainer(Base.make_sequence(n, randomize))

    def measure_append_time(self, TContainer, replay: int = 10):
        return [timeit(f"[obj.append(0) for i in range({n})]", number=replay,
                       globals={'obj': TContainer()})
                for n in self.N]

    def measure_get_time(self, TContainer, replay: int = 10):
        return [timeit(f"obj[{int(n/2)}]", number=replay,
                       globals={'obj': self.__init_container(TContainer, n)})
                for n in self.N]

    def measure_contains_time(self, TContainer, replay: int = 10,
                              randomize: bool = False):
        return [timeit(f"int(randint(0, {n})) in obj", number=replay,
                       globals={"obj": self.__init_container(TContainer, n,
                                                             randomize),
                                "randint": randint})
                for n in self.N]

    def measure_size_time(self, TContainer, replay: int = 10):
        return [timeit("len(obj)", number=replay,
                       globals={'obj': self.__init_container(TContainer, n)})
                for n in self.N]

    def measure_insert_time(self, TContainer, replay: int = 10):
        return [timeit(f"obj.insert({int(n/2)}, 0)", number=replay,
                       globals={'obj': self.__init_container(TContainer, n)})
                for n in self.N]

    def measure_pop_time(self, TContainer, replay: int = 10):
        return [timeit("obj.pop(int(n/2))", "obj = TContainer(range(n))",
                       number=replay,
                       globals={'TContainer': TContainer, 'n': n})
                for n in self.N]

    def measure_init_time(self, TContainer, replay: int = 10,
                          randomize: bool = False):
        return [timeit("TContainer(sequence)", number=replay,
                       globals={"TContainer": TContainer,
                                "sequence": Base.make_sequence(n, randomize)})
                for n in self.N]



