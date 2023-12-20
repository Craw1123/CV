#include <fstream>

#include "RawDocument.h"
#include "LogoDecorator.h"
#include "BorderDecorator.h"
#include "ToUpperDecorator.h"

int main()
{
	/* Чистый документ */
	std::ofstream rawDocFile;
	rawDocFile.open("raw_document.txt");
	DocumentBase* rawDoc = new RawDocument();
	rawDocFile << rawDoc->getText() << "\n";
	rawDocFile.close();

	/* Документ с логотипом компании */
	std::ofstream docWithBorderFile;
	docWithBorderFile.open("document_with_border.txt");
	DocumentBase* docWithBorder = new BorderDecorator(new RawDocument());
	docWithBorderFile << docWithBorder->getText() << "\n";
	docWithBorderFile.close();

	/* Документ с рамкой */
	std::ofstream docWithLogoFile;
	docWithLogoFile.open("document_with_logo.txt");
	DocumentBase* docWithLogo = new LogoDecorator(new RawDocument());
	docWithLogoFile << docWithLogo->getText() << "\n";
	docWithLogoFile.close();

	/* Документ с рамкой и логотипом */
	std::ofstream docWithBorderAndLogoFile;
	docWithBorderAndLogoFile.open("document_with_border_and_logo.txt");
	DocumentBase* docWithBorderAndLogo = new BorderDecorator(
		new LogoDecorator(new RawDocument()));
	docWithBorderAndLogoFile << docWithBorderAndLogo->getText() << "\n";
	docWithBorderAndLogoFile.close();

	/* Документ в верхнем регистре с рамкой и логотипом */
	std::ofstream docInUppercaseWithBorderAndLogoFile;
	docInUppercaseWithBorderAndLogoFile.open("document_in_uppercase_with_border_and_logo.txt");
	DocumentBase* docInUppercaseWithBorderAndLogo = new ToUpperDecorator(new BorderDecorator(
		new LogoDecorator(new RawDocument())));
	docInUppercaseWithBorderAndLogoFile << docInUppercaseWithBorderAndLogo->getText() << "\n";
	docInUppercaseWithBorderAndLogoFile.close();
}