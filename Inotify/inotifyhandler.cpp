
#include <inotifyhandler.h>
#include <exception>

InotifyHandler::InotifyHandler()
{
    moved_from_dir="";
    moved_into_dir="";
    fname_before_move="";
    fname_after_move="";

    dmoved_from_dir="";
    dmoved_into_dir="";
    dname_before_move="";
    dname_after_move="";

}

void InotifyHandler::initialize_inotify(){
    root_dir=Util::get_root_path();
    string watch_dir=this->root_dir;

    try{


        InotifyWatch watch(watch_dir, IN_CREATE |IN_DELETE |IN_MODIFY|IN_MOVE|IN_OPEN|IN_CLOSE_WRITE);
        notify.Add(watch);
        string path =watch_dir;
        this->recursively_add_watch(path);
        this->wait_for_events();

    }
    catch (InotifyException &e) {
        cerr << "Inotify exception occured: " << e.GetMessage() << endl;
        cout<< "in initialize_inotify() " << "path: "<<watch_dir<< endl;
    } catch (exception &e) {
        cerr << "STL exception occured: " << e.what() << endl;
    }catch (const boost::filesystem::filesystem_error& ex)
    {
        cout << ex.what() << '\n';
    }
    catch (...) {
        cerr << "unknown exception occured" << endl;
    }
}
/**
 * @brief InotifyHandler::wait_for_events
 * wait for events -file create, delete, rename,move, modify
 *                  -dir create , delete,reanme , move
 *
 * and call file handler for handle these events
 */
void InotifyHandler::wait_for_events(){


    moved_from_dir="";
    moved_into_dir="";
    fname_before_move="";
    fname_after_move="";

    dmoved_from_dir="";
    dmoved_into_dir="";
    dname_before_move="";
    dname_after_move="";

    string watchDir;



    for (;;) {
        try{
            notify.WaitForEvents();
        }catch(InotifyException &e){

        }

        size_t count = notify.GetEventCount();

        while (count > 0) {
            bool got_event;
            InotifyEvent event;
            try{
                got_event = notify.GetEvent(&event);
            }catch(InotifyException &e){

            }
            if (got_event) {
                string mask_str;
                event.DumpTypes(mask_str);

                string filename = event.GetName();

                watchDir=event.GetWatch()->GetPath ();

                //        cout << "[Event in " << watchDir << "] ";
                //    cout << "event mask: \"" << mask_str << "\", ";
                //cout << "filename: \"" << filename << "\"" << endl;

                /**** file created  and deleted messages********/
                if(mask_str == "IN_CREATE"){
                    if(!is_temp_file(filename)){//temp file
                        if ( createdfile.find(filename) == createdfile.end() )
                        {
                            // Item was not in the set, so put it in the set
                            createdfile.insert(filename);
                        }
                    }

                }

                if(mask_str=="IN_OPEN"){
                    if(!is_temp_file(filename)){//temp file
                        if ( openedfile.find(filename) == openedfile.end() )
                        {
                            // Item was not in the set, so put it in the set
                            openedfile.insert(filename);
                        }
                    }
                }
                if(mask_str=="IN_CLOSE_WRITE"){
                    if(!is_temp_file(filename)){//temp file
                        if ( openedfile.find(filename) != openedfile.end() && createdfile.find(filename) != createdfile.end())
                        {

                            openedfile.erase(filename);
                            createdfile.erase(filename);
                            cout << "[Event in " << watchDir << "] ";
                            cout << "New File Created: \"" << filename << "\"" << endl;

                             fh.file_create(Util::get_relative_path(watchDir),filename);
                        }
                        if ( openedfile.find(filename) != openedfile.end() && createdfile.find(filename) == createdfile.end())
                        {

                            openedfile.erase(filename);

                            cout << "[Event in " << watchDir << "] ";
                            cout << "File Modified: \"" << filename << "\"" << endl;
                             fh.file_modified(Util::get_relative_path(watchDir),filename);
                        }

                    }
                }




                if(mask_str == "IN_DELETE"){// for shift+delete button
                    if(!is_temp_file(filename)){
                        cout << "[Event in " << watchDir << "] ";
                        cout << " File Deleted: \"" << filename << "\"" << endl;
                         fh.file_delete(Util::get_relative_path(watchDir),filename);

                    }
                }

                /***end*********/

                /** add watch for new direcotry && remove watch for deleted directory**/
                if(mask_str == "IN_CREATE,IN_ISDIR"){		// new direcotry is created
                    string new_dir_name=event.GetName();		// name of new directory
                    cout << "[Event in " << watchDir << "] ";
                    cout << " Directory Created: \"" << filename << "\"" << endl;
                    string inode=CMD::get_inode_number(watchDir+"/"+new_dir_name);
                    boost::thread workerThread(&InotifyHandler::dir_created_checker,this,inode);
                    workerThread.join();



                }


                if(mask_str == "IN_DELETE,IN_ISDIR"){// direcotry deleted
                    string deletedDirName=event.GetName();
                    cout << "[Event in " << watchDir << "] ";
                    cout << "Direcotry Deleted: \"" << deletedDirName << "\"" << endl;
                    remove_watch(watchDir+"/"+deletedDirName);

                    fh.dir_deleted(Util::get_relative_path(watchDir),filename);
                }

                /******end***********/


                /** file rename ,modify and move and created(drag and drop)**/
                if(mask_str == "IN_MOVED_FROM"){
                    moved_from_dir=watchDir;
                    fname_before_move=event.GetName();
                    file_deleted=true;
                    boost::thread fileDeleteChecker(&InotifyHandler::file_deleted_checker,this,moved_from_dir,fname_before_move);
                }

                if(mask_str == "IN_MOVED_TO"){

                    moved_into_dir=watchDir;
                    fname_after_move=filename;
                   // string b_inode=CMD::get_inode_number(moved_from_dir+"/"+fname_before_move);
                    //string a_inode=CMD::get_inode_number(moved_into_dir+"/"+fname_after_move);
                    if(moved_from_dir!=""){
                        file_deleted=false;// file is not deleted it is a move/rename
                        if(moved_from_dir==moved_into_dir &&fname_before_move!=fname_after_move){// renamed
                            if(!is_temp_file(fname_before_move)){// check temp files
                                cout << "[Event in " << moved_from_dir << "] ";
                                cout << "File " +fname_before_move+ " is renamed as " +fname_after_move<< endl;
                                 fh.file_rename(Util::get_relative_path(moved_from_dir),fname_before_move,fname_after_move);
                                moved_from_dir="";
                                moved_into_dir="";
                                fname_before_move="";
                                fname_after_move="";
                            }
                        }   else{// moved
                            cout << "File "+fname_after_move+" has been moved from "+moved_from_dir+" to "+moved_into_dir<<endl;

                             fh.file_moved(Util::get_relative_path(moved_from_dir),Util::get_relative_path(moved_into_dir),fname_after_move);
                            moved_from_dir="";
                            moved_into_dir="";
                            fname_before_move="";
                            fname_after_move="";
                        }
                    }else {// drag and drop

                        cout << "[Event in " << moved_into_dir << "] ";
                        cout << " File Created: \"" << fname_after_move << "\"" << endl;
                            fh.file_create(Util::get_relative_path(moved_into_dir),fname_after_move);
                    }

                }
                /*****end****/


                /** dir rename and move and created(drag and drop)**/

                if(mask_str== "IN_MOVED_FROM,IN_ISDIR"){

                    dmoved_from_dir=event.GetWatch()->GetPath ();
                    dname_before_move=event.GetName();
                    dir_deleted=true;
                    boost::thread dirDeleteChecker(&InotifyHandler::dir_deleted_checker,this,dmoved_from_dir,dname_before_move);
                    try{
                        remove_watch(dmoved_from_dir+"/"+dname_before_move);
                    } catch (InotifyException &e) {
                        cerr << "Inotify exception occured: " << e.GetMessage() << endl;
                    } catch (exception &e) {
                        cerr << "STL exception occured: " << e.what() << endl;
                    }catch (const boost::filesystem::filesystem_error& ex)
                    {
                        cout << ex.what() << '\n';
                    } catch (...) {
                        cerr << "unknown exception occured" << endl;
                    }
                }

                if(mask_str == "IN_MOVED_TO,IN_ISDIR"){
                    dir_deleted=false;
                    dmoved_into_dir=event.GetWatch()->GetPath ();

                    dname_after_move=event.GetName();
                    if(dmoved_from_dir!=""){
                        if(dmoved_from_dir==dmoved_into_dir &&dname_before_move!=dname_after_move){
                            cout << "[Event in " << dmoved_from_dir << "] ";
                            cout << "Directory " +dname_before_move+ " is renamed as " +dname_after_move<< endl;

                            string new_watchDir=dmoved_into_dir+"/"+dname_after_move;
                            string path=new_watchDir;
                            string pfilewatchpath="";
                            string temp="";
                            try{
                                add_watch(new_watchDir);

                                boost::filesystem::recursive_directory_iterator itr(path);
                                while (itr != boost::filesystem::recursive_directory_iterator())// recursive add watch for renamed directory
                                {

                                    if(is_directory(itr->path())){
                                        temp=itr->path().string();
                                        pfilewatchpath=dmoved_from_dir+"/"+dname_before_move+temp.substr(dmoved_into_dir.length()+1+dname_after_move.length());

                                        remove_watch(pfilewatchpath);
                                        add_watch(temp);

                                    }
                                    ++itr;

                                }

                            } catch (InotifyException &e) {
                                cerr << "Inotify exception occured: " << e.GetMessage() << endl;
                            } catch (exception &e) {
                                cerr << "STL exception occured: " << e.what() << endl;
                            } catch (const boost::filesystem::filesystem_error& ex)
                            {
                                cout << ex.what() << '\n';
                            }catch (...) {
                                cerr << "unknown exception occured" << endl;
                            }
                            fh.dir_rename(Util::get_relative_path(dmoved_from_dir),dname_before_move,dname_after_move);
                            dmoved_from_dir="";
                            dmoved_into_dir="";
                            dname_before_move="";
                            dname_after_move="";
                        }else{
                            cout << "Directory "+dname_after_move+" has been moved from "+dmoved_from_dir+" to "+dmoved_into_dir<<endl;
                            string cparentdir=dmoved_into_dir+"/"+dname_after_move;
                            string path =cparentdir;

                            string pfilewatchpath="";
                            string temp="";

                            try{
                                add_watch(cparentdir);
                                boost::filesystem::recursive_directory_iterator itr(path);
                                while (itr != boost::filesystem::recursive_directory_iterator())// recursive add watch for moved directory
                                {

                                    if(is_directory(itr->path())){
                                        temp=itr->path().string();
                                        pfilewatchpath=dmoved_from_dir+"/"+temp.substr(dmoved_into_dir.length()+1);
                                        remove_watch(pfilewatchpath);
                                        add_watch(temp);

                                    }
                                    ++itr;

                                }
                            } catch (InotifyException &e) {
                                cerr << "Inotify exception occured: " << e.GetMessage() << endl;
                            } catch (exception &e) {
                                cerr << "STL exception occured: " << e.what() << endl;
                            }catch (const boost::filesystem::filesystem_error& ex)
                            {
                                cout << ex.what() << '\n';
                            }
                            catch (...) {
                                cerr << "unknown exception occured" << endl;
                            }




                             fh.dir_moved(Util::get_relative_path(dmoved_from_dir)+dname_before_move,Util::get_relative_path(dmoved_into_dir)+dname_after_move);// move only main dir so in server all child should move
                            dmoved_from_dir="";
                            dmoved_into_dir="";
                            dname_before_move="";
                            dname_after_move="";

                        }

                    }else{
                        add_watch(dmoved_into_dir+"/" +dname_after_move);
                        recursively_add_watch(dmoved_into_dir+"/" +dname_after_move);
                        cout << "[Event in " << dmoved_into_dir << "] ";
                        cout << " Directory Created: \"" << dname_after_move << "\"" << endl;
//                        string temp_path=Util::get_parent_path(path);
//                        string temp_dirname=Util::get_filename_from_path(path);
//                        fh.dir_created(Util::get_relative_path(temp_path),temp_dirname);
                    }
                }
                /*****end******/

            }

            count--;
        }
    }
}


/**
 * @brief InotifyHandler::add_watch
 *add inotify watch for a directory if already it has not been added
 * @param path
 */
void InotifyHandler::add_watch(string path){

    try{
        if(notify.FindWatch(path)==NULL){
            notify.Add(new InotifyWatch(path, IN_CREATE |IN_DELETE |IN_MODIFY|IN_MOVE|IN_OPEN|IN_CLOSE_WRITE));
        }
    } catch (InotifyException &e) {
        cerr << "Inotify exception occured: " << e.GetMessage() << endl;
        cout<< "in add_watch() " << "path: "<<path<< endl;
    } catch (exception &e) {
        cerr << "STL exception occured: " << e.what() << endl;
    } catch (const boost::filesystem::filesystem_error& ex)
    {
        cout << ex.what() << '\n';
    }catch (...) {
        cerr << "unknown exception occured" << endl;
    }
}

void InotifyHandler::remove_watch(string path){
    try{
        if(notify.FindWatch(path)!=NULL){
            notify.Remove(notify.FindWatch(path));
        }
    } catch (InotifyException &e) {
        cerr << "Inotify exception occured: " << e.GetMessage() << endl;
        cout<< "in remove_watch() " << "path: "<<path<< endl;
    } catch (exception &e) {
        cerr << "STL exception occured: " << e.what() << endl;
    } catch (...) {
        cerr << "unknown exception occured" << endl;
    }
}
/**
 * @brief InotifyHandler::recursively_add_watch
 *this function will iterate through a given directory and will add inotify watch to all its sub directories
 * @param dir
 * parent direcotory path
 */
void InotifyHandler::recursively_add_watch(string dir){
    string path =dir;
    try{
        boost::filesystem::recursive_directory_iterator itr(path);

        while (itr != boost::filesystem::recursive_directory_iterator())// recursive add watch for root
        {
            if(boost::filesystem3::is_directory(itr->path())){

                add_watch(itr->path().string());


            }
            ++itr;

        }
    }catch(boost::filesystem::filesystem_error &ex){
        cerr << "Boost file system exception occured: " << ex.what() << endl;
    }
}


/**
 * @brief InotifyHandler::DirCreatedChecker
 *thread function
 *checck whether a directory is completely created(including files and sub dirs)
 * @param path
 *path of the directory
 * @return
 */
void InotifyHandler::dir_created_checker(string inode){
    boost::posix_time::seconds Time(4);
    size_t previous_size=-1;
    size_t current_size=-1;
    //add_watch(path);
    boost::this_thread::sleep(Time);// wait for renaming
    string path=CMD::get_path_from_inode(inode);
    previous_size=Util::get_dir_size(path);// not thread safe

    while(true){

        path=CMD::get_path_from_inode(inode);
        current_size=Util::get_dir_size(path);
        if(previous_size==current_size){// dir completely created
            //std::cout<< current_size<< std::endl;
            break;
        }else {
            previous_size=current_size;
        }
        boost::this_thread::sleep(Time);// wait for copying
    }
    std::cout<< "dir completely created"<< std::endl;
    //std::cout<< path<< std::endl;
    // to do -lock  parent dir
    add_watch(path);
    //(path);

    string temp_path=Util::get_parent_path(path);
    string temp_dirname=Util::get_filename_from_path(path);
    fh.dir_created(Util::get_relative_path(temp_path),temp_dirname);
    try{

        boost::filesystem::recursive_directory_iterator itr(path);

        while (itr != boost::filesystem::recursive_directory_iterator())// recursive add watch for root
        {
            if(boost::filesystem3::is_directory(itr->path())){
                std::cout << "dir created :" << itr->path()<< std::endl;

                add_watch(itr->path().string());
                temp_path=Util::get_parent_path(itr->path().string());
                temp_dirname=Util::get_filename_from_path(itr->path().string());
                //cout <<Util::get_relative_path(temp_path)<< endl;
                //cout <<temp_dirname<< endl;
                fh.dir_created(Util::get_relative_path(temp_path),temp_dirname);

            }else {
                std::cout << "file created :" << itr->path()<< std::endl;
                temp_path=Util::get_parent_path(itr->path().string());
                temp_dirname=Util::get_filename_from_path(itr->path().string());
                 fh.file_create(Util::get_relative_path(temp_path),temp_dirname);
            }
            ++itr;

        }
    } catch (const boost::filesystem::filesystem_error& ex)
    {
        cout << ex.what() << '\n';
    }
    // to do -release lock

}


/**
 * @brief InotifyHandler::FileCreatedChecker
 *thread function
 *check whether a file is completely created
 * @param path
 * path of the file
 * @return
 */
void InotifyHandler::file_created_checker(string path){

    boost::posix_time::seconds Time(4);
    size_t previous_size=-1;
    size_t current_size=-1;

    previous_size=Util::get_file_size(path);

    while(true){

        boost::this_thread::sleep(Time);
        current_size=Util::get_file_size(path);
        if(previous_size==current_size){// file completely created
            break;
        }else {
            previous_size=current_size;
        }
    }

    // call file handler
    std::cout<< "file completely created"<< std::endl;
    string temp_path=Util::get_parent_path(path);
    string temp_filename=Util::get_filename_from_path(path);
    fh.file_modified(Util::get_relative_path(temp_path),temp_filename);
}

void InotifyHandler::file_deleted_checker(string path, string name){
    boost::posix_time::seconds Time(4);
    boost::this_thread::sleep(Time);
    if(file_deleted){
        cout << "[Event in " << path << "] ";
        cout << " File Deleted: \"" << name << "\"" << endl;
         fh.file_delete(Util::get_relative_path(path),name);

        moved_from_dir="";
        moved_into_dir="";
        fname_before_move="";
        fname_after_move="";

    }
}

void InotifyHandler::dir_deleted_checker(string path, string name){
    boost::posix_time::seconds Time(4);
    boost::this_thread::sleep(Time);
    if(dir_deleted){
        remove_watch(path +"/"+name);

        cout << "[Event in " << path << "] ";
        cout << " Directory Deleted: \"" << name << "\"" << endl;

        fh.dir_deleted(Util::get_relative_path(path),name);
        dmoved_from_dir="";
        dmoved_into_dir="";
        dname_before_move="";
        dname_after_move="";
    }
}

bool InotifyHandler::is_temp_file(string filename){
    if(filename.find("~")!= string::npos ||filename.find(".goutputstream")!= string::npos ){//temp file
        return true;
    }
    return false;
}

