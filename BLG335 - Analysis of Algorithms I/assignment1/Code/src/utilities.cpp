#include "tweet.h"

std::vector<Tweet> readTweetsFromFile(const std::string& filename) 
{
    std::vector<Tweet> tweets;

    std::fstream f;
    f.open(filename);

    std::string title;
    getline(f, title);

    std::string line;
    while(getline(f,line)){
        std::stringstream s(line);
        std::string tweetID_str, favoriteCount_str, retweetCount_str;

        if (getline(s, tweetID_str, ',') && getline(s, retweetCount_str, ',') && getline(s, favoriteCount_str, ',')){
            long long tweetID = stol(tweetID_str);
            int favoriteCount = stoi(favoriteCount_str);
            int retweetCount = stoi(retweetCount_str);

            Tweet tweet;
            tweet.tweetID = tweetID;
            tweet.favoriteCount = favoriteCount;
            tweet.retweetCount = retweetCount;
            tweets.push_back(tweet);
        }
    }

    return tweets;
}

void writeTweetsToFile(const std::string& filename, const std::vector<Tweet>& tweets) 
{
    std::fstream f;
    f.open(filename);

    f << "tweetID" << "," << "retweet_count" << "," << "favorite_count" << "\n"; 

    for(Tweet tweet : tweets){
        f << tweet.tweetID << "," << tweet.retweetCount << "," << tweet.favoriteCount << "\n";
    }
}