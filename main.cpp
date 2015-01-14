#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <functional>

using namespace std;


// Store one review:
class Review
{
public:
    int movieID;
    int userID;
    int movieRating;
    Review(int movie, int user, int rating)
    {
        movieID = movie;
        userID = user;
        movieRating = rating;
    }
};

// Stores one movie:
class Movie
{
public:
    int movieID;
    string movieName;
    
    Movie(int movie, string name)
    {
        movieID = movie;
        movieName = name;
        totalRating = 0;
        numRatings = 0;
        avgRating = 0;
    }
    
    
    //fields & functions I added to movie class
    int totalRating;
    int numRatings;
    double avgRating;
    int ratings[4] = {0,0,0,0};
    
    double agverage(){
        return (double)totalRating/(double)numRatings;
    }
};

//stores user data
class User
{
public:
    int id;
    int numRatings;
    
    User(int i){
        id = i;
        numRatings = 1;
    }
    
    void addReview(){
        numRatings++;
    }
};

// Parses one line of the reviews file and returns a Review object:
Review ParseReview(string line, vector<Movie>& vec, vector<User>& users)
{
    istringstream strstream(line);
    string movieIDstr, userIDstr, ratingstr;
    getline(strstream, movieIDstr, ',');
    getline(strstream, userIDstr, ',');
    getline(strstream, ratingstr, ',');
    int movieID = atoi(movieIDstr.c_str());
    int userID = atoi(userIDstr.c_str());
    int movieRating = atoi(ratingstr.c_str());
    
    //update movie list
    vec[movieID-1].totalRating += movieRating;
    vec[movieID-1].numRatings++;
    vec[movieID-1].avgRating = vec[movieID-1].agverage();
    vec[movieID-1].ratings[movieRating-1]++;
    
    //return review
    return Review(movieID, userID, movieRating);
}

// Parses one line of the movies file and returns a Movie object:
Movie ParseMovie(string line)
{
    size_t comma = line.find(',');
    line = line.substr(0, line.size()-1);
    int movieID = atoi(line.substr(0, comma).c_str());
    string movieName = line.substr(comma + 1);
    return Movie(movieID, movieName);
}

//start my functions' prototyping (below main)
    bool movie_sort (Movie left, Movie right) { return (left.avgRating>right.avgRating); }
    bool review_sort (Review left, Review right) { return (left.userID>right.userID); }
    bool user_sort (User left, User right) { return (left.numRatings>right.numRatings); }
    int searchMovie(vector<Movie>m);
    void getTop10(vector<Movie> m);
    void getTopUsers(vector<Review> r, vector<User>& u);


int main(int argc, const char * argv[]) {
    
    //read in movies
    ifstream input("movies.txt");
    string line;
    vector<Movie> movies;
    while(getline(input, line))
    {
        movies.push_back(ParseMovie(line));
    }
    input.close();
    
    //read in reviews
    ifstream input2("reviews1.txt");
    vector<Review> reviews;
    vector<User> users;
    while(getline(input2, line))
    {
        reviews.push_back(ParseReview(line, movies, users));
    }
    
    //get and output the top 10 movies
    getTop10(movies);
    
    //get and output the top 10 users
    getTopUsers(reviews, users);
    
    //propt user for movie id and give back movie info
    cout << endl << endl << endl << "SEARCH FOR MOVIES (or type 0 to quit):" << endl;
    while (searchMovie(movies)) {
        continue;
    }
    
    
}

int searchMovie(vector<Movie>m){
    //prompt fot movie id
    int mId;
    cout << endl << "Enter a movie id: ";
    cin >> mId;
    
    if (mId == 0) { //if 0 is entered quit loop.
        return 0;
    }
    if (mId > m.size() && mId < 0) { //if movie does not exist, say so
        cout << endl << "Movie with id " << mId << " does not exist!" << endl;
    }else{ //if movie is selected ouput info
        cout << endl << left <<setw(6) << "ID" << setw(40) << "Name" << setw(10) << "Rating" << setw(14) << "# of Ratings" <<setw(8) << "1 Star" << setw(8) << "2 Star" << setw(8) << "3 Star" << setw(8) << "4 Star" << setw(8) << "5 Star" << endl;
        cout << "----- --------------------------------------- --------- ------------- ------- ------- ------- ------- ------- " << endl;
            cout << left <<setw(6) << mId << setw(40) << m[mId-1].movieName << setw(10) << m[mId-1].avgRating << m[mId-1].numRatings << setw(8) << setw(14) << m[mId-1].ratings[0] << setw(8) << m[mId-1].ratings[1] << setw(8) << m[mId-1].ratings[2] << setw(8) << m[mId-1].ratings[3] << setw(8) << m[mId-1].ratings[4] << endl;
    }
    
    
    return 1;
}

void getTop10(vector<Movie> m){
    partial_sort(m.begin(), m.begin()+10, m.end(), movie_sort);
    cout << "TOP 10 MOVIES:" << endl << endl;
    cout << left << setw(5) << "Rank" <<setw(6) << "ID" << setw(40) << "Name" << setw(10) << "Rating" << "# of Ratings" <<  endl;
    cout << "---- ----- --------------------------------------- --------- ------------" << endl;
    int i;
    for (i=0; i<10; i++) {
        cout << left << setw(5) << i+1 <<setw(6) << m[i].movieID << setw(40) << m[i].movieName << setw(10) << m[i].avgRating << m[i].numRatings << endl;
    }
}

void getTopUsers(vector<Review> r, vector<User>& u){
    //sort reviews by username
    sort(r.begin(), r.end(), review_sort);
    
    int i;
    //initialize user vector
    for ( i = 0; i < r.size(); i++) {
        if (i == 0){
            u.push_back(User(r[i].userID));
        }else if (r[i].userID == r[i-1].userID){
            u[u.size()-1].addReview();
        }else{
            u.push_back(User(r[i].userID));
        }
    }
    
    partial_sort(u.begin(), u.begin()+10, u.end(), user_sort);
    
    //output results
    cout << endl << endl << endl << "TOP 10 USERS:" << endl;
    cout << endl << setw(5) << "Rank" << setw(10) << "ID" << "Number of Reviews"<< endl;
    cout << "---- --------- -----------------" <<endl;
    for (i=0; i<10; i++) {
        cout << setw(5) << i+1 << left << setw(10) << u[i].id << u[i].numRatings << endl;
    }
}

