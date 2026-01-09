package JavaDataModel

val ISolution.inputFileName: String?
    get() = this.javaClass.annotations.filterIsInstance<AoCSolution>().firstOrNull()
        ?.let { "Day%02d/input.txt".format(it.day) }
