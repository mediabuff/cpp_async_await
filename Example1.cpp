#include "asio_helper.hpp"
#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>



int main()
{
    boost::asio::io_service io;
    boost::asio::deadline_timer t(io, boost::posix_time::seconds(1));


    try{


        // This allows us to  do our await magic
        asio_helper::do_async([&](asio_helper::async_helper helper){

            // Notice how we can use a real loop
            for(int i = 0; i < 5; i++){

                // Sets up the callback so it will resume our function
                auto cb = helper.make_callback([&](const boost::system::error_code& ec)->boost::system::error_code{return ec;});

                // Call the async function with the results of make_callback
                t.async_wait(cb);

                // When our callback function is called, it will cause us to resume here
                // Await will return whatever we make our callback return, in this case the error_code
                auto ec = helper.await<boost::system::error_code>();

                // Print a message about the timer
                std::cout << "Timer went off " << (i+1) << " times with ec = " << ec.message() << std::endl;

                // Set up a new expiration for the timer
                t.expires_from_now(boost::posix_time::seconds(1));

            }

        });

     io.run();

    }
    catch(std::exception& e){
        std::cerr << e.what() << "\n";
    }



    return 0;
}
