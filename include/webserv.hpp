#pragma once

# include <iostream>

class Web
{
	private:

	public:
		Web();
		~Web();

		void check(char *argv) const;
		void run() const;
		void clean() const;
};
