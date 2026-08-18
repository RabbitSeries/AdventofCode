package JavaDataModel

val ISolution.inputFileName: String?
    get() = this.javaClass.annotations.filterIsInstance<AoCSolution>().firstOrNull()
        ?.let { "Day%02d/input.txt".format(it.day) }

val ISolution.yearName: Int?
    get() = this.javaClass.annotations.filterIsInstance<AoCSolution>().firstOrNull()?.year

val ISolution.dayName: Int?
    get() = this.javaClass.annotations.filterIsInstance<AoCSolution>().firstOrNull()?.day
