#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <vector>

using namespace std;

enum { invalidPuzzle, completePuzzle, incompletePuzzle };

class Cell {

public:
	Cell(unsigned v, unsigned max = false);

	bool CantBe(Cell &cantBeThis);
	unsigned int Guess();
	inline unsigned int &Value() { return value; }
	inline unsigned int Certainty() { return possibles.size(); }
	void Display();

private:
	unsigned					value;
	vector<unsigned int>		possibles;
};

class Puzzle {

public:
	Puzzle(unsigned w, unsigned h, unsigned scw, unsigned sch);
	Puzzle(const char * const fileName);
	Puzzle(FILE *stream);

	unsigned int Solve();
	unsigned int Guess();

	Cell &CellAt(const unsigned int x, const unsigned int y);
	void Display(const bool displayNewlines, const bool displayPoss);
    
    bool humanReadable;

private:
	void ReadFile(FILE *stream);

	unsigned int				width, height;
	unsigned int				subCellWidth, subCellHeight;
	unsigned int				maximumPossibilities, certain, invalid;
	unsigned int				solveTarget;
	unsigned int				solvedThisPass;
	unsigned int				passes;
	vector<Cell>				cells;
};

Puzzle::Puzzle(unsigned w, unsigned h, unsigned scw, unsigned sch) : width(w), height(h), subCellWidth(scw), subCellHeight(sch) {
    
	// Initialise
	certain = 0;
	invalid = 0;
	passes = 0;

	// Calculate the maximum values
	maximumPossibilities = subCellWidth * subCellHeight;
	solveTarget = width * height;

	// Put blank data into cells
	for (int value = 0; value < width * height; value++) {
		cells.push_back(Cell(0, maximumPossibilities));
	}
}

Puzzle::Puzzle(FILE *stream) {
    
	ReadFile(stream);
}

void Puzzle::ReadFile(FILE *stream) {
    
	// Initialise
	certain = 0;
	invalid = 0;
	passes = 0;

	if (!stream) throw "file not found";

	// Read dimensions
	if (fscanf(stream, "dims %d %d %d %d\n", &width, &height, &subCellWidth, &subCellHeight) != 4) throw "can't find dims in stream";

	// Calculate the maximum values
	maximumPossibilities = subCellWidth * subCellHeight;
	solveTarget = width * height;

	int count = 0, expected = width * height;

	// Read data into cells
	for (int value = 0; fscanf(stream, "%d", &value) != EOF;) {
		cells.push_back(Cell(value, maximumPossibilities));
		count++;
	}

	if (count > expected) throw "too much data in the stream";
	if (count < expected) throw "not enough data in the stream";
}

Puzzle::Puzzle(const char * const fileName) {
    
	FILE *file = fopen(fileName, "r");

	ReadFile(file);

	fclose(file);
}

void Puzzle::Display(const bool displayNewlines = true, const bool displayPoss = false) {
    
	vector<Cell>::iterator	iter;
	int						t;

	for (iter = cells.begin(), t = 1; iter != cells.end(); iter++, t++) {

        if (displayPoss) iter->Display();
		else printf("%d ", iter->Value());
		if (displayNewlines && !(t % width)) printf("\n");
	}

	printf("\n%d certain %d invalid %d passes %d solves\n", certain, invalid, passes, solvedThisPass);
}

unsigned int Puzzle::Solve() {
    
	// Initialise
	certain = 0;
	invalid = 0;
	solvedThisPass = 0;

	for (unsigned int y = 0; y < height; y++) {

		// This is for keeping track of the sub cell y coordinate (left top corner of sub cell)
		unsigned int cellY = (y / subCellHeight) * subCellHeight;

		for (unsigned int x = 0; x < width; x++) {

			// This is for keeping track of the sub cell x coordinate (left top corner of sub cell)
			unsigned int cellX = (x / subCellWidth) * subCellWidth;

			Cell &currentCell = CellAt(x, y);

			// Keep hold of the value before solving
			unsigned int cellValue = currentCell.Value();

			// Check current cell against its row
			for (unsigned int row = 0; row < width; row++) {

				// Don't check against current cell
				if (row == x) continue;

				if (!currentCell.CantBe(CellAt(row, y))) invalid++;
			}

			// Check current cell against its column
			for (unsigned int col = 0; col < height; col++) {

				// Don't check against current cell
				if (col == y) continue;

				if (!currentCell.CantBe(CellAt(x, col))) invalid++;
			}

			// Check current cell against its sub cell
			for (unsigned int cell = 0; cell < maximumPossibilities; cell++) {

				unsigned int checkX = cellX + (cell % subCellWidth);
				unsigned int checkY = cellY + (cell / subCellWidth);

				// Don't check against current cell
				if (checkX == x && checkY == y) continue;

				if (!currentCell.CantBe(CellAt(checkX, checkY))) invalid++;
			}

			// Check if we have solved the value
			if (currentCell.Value()) { certain++; if (!cellValue) solvedThisPass++; }
		}
	}

	passes++;

	// Bail out if the puzzle isn't valid any more
	if (invalid) return invalidPuzzle;

	// If we haven't solved the whole puzzle yet...
	if (certain < solveTarget) {

		// If some cells were solved this time, just solve again
		if (solvedThisPass) return Solve();

		// Otherwise, have a guess at a probable value
		else return Guess();
	}

	Display(humanReadable);

	return completePuzzle;
}

unsigned int Puzzle::Guess() {

	unsigned int result;

	Cell			*mostCertainCell = NULL; // RJH Move this to Solve loop & keep reference
	unsigned int	highestCertainty = maximumPossibilities + 1;

	// Find the most certain cell
	for (unsigned int y = 0; y < height; y++) {

		for (unsigned int x = 0; x < width; x++) {

			// Get a cell
			Cell &currentCell = CellAt(x, y);
			// and its value
			unsigned int cellValue = currentCell.Value();

			// Don't guess at a firm value
			if (cellValue) continue;

			// Get the number of possibilities for the cell
			unsigned int numPossibilities = currentCell.Certainty();

			// Find the cell with the lowest number of possibilities
			if (numPossibilities < highestCertainty) {
				// Save the most certain cell
				mostCertainCell = &currentCell;
				highestCertainty = numPossibilities;
			}
		}
	}

	if (!mostCertainCell) return completePuzzle;

	// Try to solve for all values of the most certain cell
	for (bool finished = false; !finished; ) {

		// This cell contains the current guess
		Cell c(mostCertainCell->Guess());

		// Clone a new puzzle
		Puzzle p(*this);

		// and try to solve it
		result = p.Solve();

		if (result == invalidPuzzle) {
			// This guess resulted in an invalid puzzle so it can't be the right guess
			mostCertainCell->Value() = 0;
			mostCertainCell->CantBe(c);
		} else {
			// The puzzle is solved
			break;
		}

		if (mostCertainCell->Certainty() < 1) break;
	}

	return result;
}

Cell &Puzzle::CellAt(const unsigned int x, const unsigned int y) {
    
	return cells[y * width + x];
}

Cell::Cell(unsigned v, unsigned max) : value(v) {
    
	// Check the cell value is sane
	if (max && v > max) throw "cell value too great";

	// If the value isn't specified, this cell could be anything
	if (!value && max)
		for (int i = 1; i <= max; i++) possibles.push_back(i);
}

bool Cell::CantBe(Cell &cantBeThis) {
    
	vector<unsigned int>::iterator iter;

	// Check if there has been an error (we have a value and we're being told that it can't be that value)
	if (value && value == cantBeThis.value) return false;

	// erase that possibility
	for (iter = possibles.begin(); iter != possibles.end(); iter++)

		if (*iter == cantBeThis.value) {

			possibles.erase(iter);

			// Check if we have reached a definite value
			if (possibles.size() == 1) value = possibles[0];

			break;
		}

	return true;
}

unsigned int Cell::Guess() {

	value = possibles[rand() % possibles.size()];

	return value;
}

void Cell::Display() {
    
	vector<unsigned int>::iterator iter;

	if (value) printf(" > %d < ", value);
	else {
		printf(" (");
		for (iter = possibles.begin(); iter != possibles.end(); iter++)
			printf("%d ", *iter);
		printf(") ");
	}
}

int main(int argc, char **argv) {

	struct timeval      t, u;
	unsigned int        result = 0;
	int                 ch;
    bool                humanReadable = false;
	bool                generateFlag = false;
	char                *fileName = NULL;

	// Process command line options
	while ((ch = getopt(argc, argv, "hgf:")) != -1) {

		switch (ch) {
			case 'h':
				humanReadable = true;
			break;

			case 'g':
				generateFlag = true;
			break;

			case 'f':
				fileName = optarg;
			break;

			case '?':
				printf("usage\n");
				return 1;
			break;
		}
	}

	srand(time(NULL));

	try {

		if (generateFlag) {
			printf("generate puzzle\n");
        } else if (fileName) {
			// A filename was passed as the argument
			Puzzle p(fileName);
            p.humanReadable = humanReadable;
			gettimeofday(&t, NULL);

			result = p.Solve();

		} else {
			// No argument. Use data from stdin
			Puzzle p(stdin);
            p.humanReadable = humanReadable;
			gettimeofday(&t, NULL);

			result = p.Solve();
		}
	}

	catch (const char *message) { fprintf(stderr, "Error : %s\n", message); return 1; }

	if (!result) return 1;

	gettimeofday(&u, NULL);
	float elapsed_time = float(u.tv_sec - t.tv_sec) + (float(u.tv_usec - t.tv_usec) * 0.000001);
	printf("solving took %f seconds\n", elapsed_time);

	return 0;
}
