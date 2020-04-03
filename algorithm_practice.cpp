#include <iostream>
#include <list>
using namespace std;

struct Disctict
{
	double m_population;
	bool m_party;
	double m_percentage;
	list<Disctict *> m_neighbors;
};

int main(){

	//Creating dummy districts
	Disctict dis1, dis2, dis3, dis4, dis5, dis6, dis7, dis8;

	//Each district is given a default party affiliation
	dis1.m_party = true;
	dis2.m_party = false;
	dis3.m_party = true;
	dis4.m_party = false;
	dis5.m_party = true;
	dis6.m_party = false;
	dis7.m_party = true;
	dis8.m_party = false;

	//Each district is given a population of 10
	dis1.m_population = 10;
	dis2.m_population = 10;
	dis3.m_population = 10;
	dis4.m_population = 10;
	dis5.m_population = 10;
	dis6.m_population = 10;
	dis7.m_population = 10;
	dis8.m_population = 10;

	//Each district is given a dominant percentage of 70%
	dis1.m_percentage = .8;
	dis2.m_percentage = .8;
	dis3.m_percentage = .8;
	dis4.m_percentage = .8;
	dis5.m_percentage = .8;
	dis6.m_percentage = .8;
	dis7.m_percentage = .8;
	dis8.m_percentage = .8;

	//The districts are given information about their neighbors
	/*Note, these connections are defined by a map that I have
	dawn out on paper */
	dis1.m_neighbors.push_front(&dis7);
	dis1.m_neighbors.push_front(&dis8);
	dis1.m_neighbors.push_front(&dis5);
	dis2.m_neighbors.push_front(&dis6);
	dis2.m_neighbors.push_front(&dis4);
	dis3.m_neighbors.push_front(&dis4);
	dis3.m_neighbors.push_front(&dis6);
	dis4.m_neighbors.push_front(&dis2);
	dis4.m_neighbors.push_front(&dis3);
	dis5.m_neighbors.push_front(&dis1);
	dis5.m_neighbors.push_front(&dis6);
	dis6.m_neighbors.push_front(&dis2);
	dis6.m_neighbors.push_front(&dis3);
	dis6.m_neighbors.push_front(&dis5);
	dis7.m_neighbors.push_front(&dis8);
	dis7.m_neighbors.push_front(&dis1);
	dis8.m_neighbors.push_front(&dis7);
	dis8.m_neighbors.push_front(&dis1);

	/*The algorithm will continue until there are only
	two districts left, dis1 and dis2. For now, we are 
	going to count the number of districts being added.*/
	int addDis = 0;
	int time = 0;

	/*Iterator for the list has to be seperate because otherwise
	I end up adding the same thing over and over again */
	list<Disctict *>::iterator it = dis1.m_neighbors.begin();
	list<Disctict *>::iterator it2 = dis2.m_neighbors.begin();
	
	while(addDis < 7 && time < 20){
		time++;
		//While the district still has a choice to make
		bool choice = true;

		while(choice){

			/*Each district will go through its neighbors to 
			find one to add */
			for(it; it!=dis1.m_neighbors.end();++it){

				/*If the neighboring district, really the precinct,
				is of the same party, then add it and continue */
				if (dis1.m_party == (*it)->m_party && &dis1 != *it)
				{

					/*So, to add the district, add the population, fix
					the percentage, and fix the neighbors as needed */

					int newTotal = ((dis1.m_population * dis1.m_percentage) + ((*it)->m_percentage * (*it)->m_population));
					dis1.m_population = dis1.m_population + (*it)->m_population;
					dis1.m_percentage = newTotal / dis1.m_population;
					dis1.m_neighbors.merge((*it)->m_neighbors);
					dis1.m_neighbors.unique();

					/*A choice no longer needs to be made, and a
					district has been added, so break and move on */
					choice = false;
					addDis += 1;
					++it;
					break;
				}

			}
			choice = false;
		}

		choice = true;
		while(choice){

			for(it2; it2!=dis2.m_neighbors.end();++it2){

				/*If the neighboring district, really the precinct,
				is of the same party, then add it and continue */
				if ((dis2.m_party == (*it2)->m_party) && &dis2 != *it2)
				{
					/*So, to add the district, add the population, fix
					the percentage, and fix the neighbors as needed */
					
					int newTotal = ((dis2.m_population * dis2.m_percentage) + ((*it2)->m_percentage * (*it2)->m_population));
					dis2.m_population = dis2.m_population + (*it2)->m_population;
					dis2.m_percentage = newTotal / dis2.m_population;
					dis2.m_neighbors.merge((*it2)->m_neighbors);
					dis2.m_neighbors.unique();

					/*A choice no longer needs to be made, and a
					district has been added, so break and move on */
					choice = false;
					addDis += 1;
					++it2;
					break;
				}

			}
			choice = false;

		}

	}

	cout << "District 1 info:" << endl;
	cout << "Party: " << dis1.m_party << endl;
	cout << "Population: " << dis1.m_population << endl;
	cout << "Percentage: " << dis1.m_percentage << endl;

	cout << "District 2 info: " << endl;
	cout << "Party: " << dis2.m_party << endl;
	cout << "Population: " << dis2.m_population << endl;
	cout << "Percentage: " << dis2.m_percentage << endl;

	return 1;
}
