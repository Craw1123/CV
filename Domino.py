class Domino:
    OUTPUT_FORMAT = "{i} {rotation}"
    WITH_ROTATION = '+'
    NO_ROTATION = '-'

    def play_domino(self, input: list) -> list:
        # TODO: Not yet implemented
        # Example of output formatting
        return [
            self.OUTPUT_FORMAT.format(i=2, rotation=self.NO_ROTATION),
            self.OUTPUT_FORMAT.format(i=5, rotation=self.WITH_ROTATION),
            self.OUTPUT_FORMAT.format(i=1, rotation=self.WITH_ROTATION),
            self.OUTPUT_FORMAT.format(i=3, rotation=self.WITH_ROTATION),
            self.OUTPUT_FORMAT.format(i=4, rotation=self.NO_ROTATION),
        ]


if __name__ == '__main__':
    domino = Domino()
    input = [
        (1, 2),
        (2, 4),
        (2, 4),
        (6, 4),
        (2, 1)
    ]
    output = domino.play_domino(input)
    print('\n'.join(output))
