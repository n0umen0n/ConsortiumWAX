
#include <eosio/action.hpp>
#include <eosio/transaction.hpp>
#include <eosio/asset.hpp>
#include <eosio/contract.hpp>
#include <eosio/dispatcher.hpp>
#include <eosio/eosio.hpp>
#include <eosio/name.hpp>
#include <eosio/singleton.hpp>
#include <cmath>
#include <numeric>
#include <vector>


using namespace eosio;
using namespace std;
using std::vector;


class [[eosio::contract("waxconso")]] waxconso : public contract {

public:
	using contract::contract;



TABLE datapoints {
    uint64_t id;
    name owner; 
    uint64_t value;
    uint64_t median;
    time_point timestamp;

    uint64_t primary_key() const {return id;}
    uint64_t by_timestamp() const {return timestamp.elapsed.to_seconds();}
    uint64_t by_value() const {return value;}

  };

   typedef eosio::multi_index<"datapoints"_n, datapoints,
      indexed_by<"value"_n, const_mem_fun<datapoints, uint64_t, &datapoints::by_value>>, 
      indexed_by<"timestamp"_n, const_mem_fun<datapoints, uint64_t, &datapoints::by_timestamp>>> datapointstable;



         struct [[eosio::table]] pairs_struct {
            uint64_t id;

            asset             supply;
            extended_asset    pool1;
            extended_asset    pool2;

            int fee;
            name fee_contract;

            uint64_t primary_key() const { return id; }
           
            
         };


typedef eosio::multi_index<"pairs"_n, pairs_struct> pairs_structs;







TABLE feedata {

float votefee;

float pollfee;

float communityfee;

name community;

auto primary_key() const { return community.value; }
    };

typedef eosio::multi_index<name("feedata"), feedata > feedatatb;







TABLE votercounts {

name voter;

uint64_t id;


auto primary_key() const { return id; }

uint64_t by_secondary( ) const { return voter.value; }

    };

typedef eosio::multi_index<name("voterincoms"), votercounts,
eosio::indexed_by<"byvotr"_n, eosio::const_mem_fun<votercounts, uint64_t, &votercounts::by_secondary>>> mituvoterit;



TABLE divperiod {

  time_point_sec periodstart;

  uint64_t claimperiod;  

  int64_t periodfreq;


};

typedef eosio::singleton<"divperiod"_n, divperiod> divperiod_def;


TABLE creatordata {

name creator;

name community;

auto primary_key() const { return community.value; }
    };

typedef eosio::multi_index<name("commcreator"), creatordata > creatordatatb;






TABLE parimad {

name governor;

asset rewardsreceived;

auto primary_key() const { return governor.value; }


    };

typedef eosio::multi_index<name("parimad"), parimad > aintparimad;





// questions
TABLE kysimused {
    
     uint64_t pollkey;
    
     name community;

     name creator;

     vector <uint64_t> totalvote;

     vector <string> answers;

     string question;

     string description;

     uint8_t nrofvoters = 0;

     uint64_t sumofallopt = 0;

     string uniqueurl;

     time_point_sec timecreated;


    auto primary_key() const { return pollkey; }

    uint64_t by_secondary( ) const { return community.value; }

    };


  typedef eosio::multi_index<"kysimused"_n, kysimused,
  eosio::indexed_by<"bycomju"_n, eosio::const_mem_fun<kysimused, uint64_t, &kysimused::by_secondary>>> kysimuste;



/*
TABLE usrpollz {

    
    uint64_t pollkey;
        
    auto primary_key() const { return pollkey; }

    };
     typedef eosio::multi_index<name("theusrpoll"), usrpollz > kasutajapolz;
*/



TABLE usrpollopt {

    uint32_t pollkey;
        
    auto primary_key() const { return pollkey; }

    };
     typedef eosio::multi_index<name("usrpollopt"), usrpollopt > kaspolzopt;





TABLE personstaked {

    name staker;

    asset staked;

    auto primary_key() const { return staker.value; }

    };

    typedef eosio::multi_index<name("personstaked"), personstaked > personstkd;


TABLE voterstatzi {

    name voter;

    time_point_sec first_vote_time;

    uint64_t dailyvoted;

    auto primary_key() const { return voter.value; }

    };

    typedef eosio::multi_index<name("voterstatzi"), voterstatzi > voteristaati;




TABLE indtotalstkh {
        
    name staker;

    asset totalstaked;

    
    auto primary_key() const { return staker.value; }
    };
    typedef eosio::multi_index<name("indtotalstkh"), indtotalstkh > indstakud;



TABLE totalstk {

  asset totalstaked = {int64_t (00000), symbol ("POLL", 4)};
};
typedef eosio::singleton<"totalstk"_n, totalstk> totalstk_def;


TABLE commdata {
        
    name community;

    uint64_t totalvoters = 0;

    uint64_t totaltokensvoted = 0;

    string communityname;

    string description;

    string backgroundurl;

    string tokenurl;

    string tokensymbol;

    uint64_t toppoll;

    asset staked;

    symbol toksym;

    name contractname;

    uint64_t totalcirculation;

    uint64_t decimals;

    auto primary_key() const { return community.value; }
    };
    typedef eosio::multi_index<name("commdata"), commdata > comdata;




         TABLE account {
            asset    balance;

            uint64_t primary_key()const { return balance.symbol.code().raw(); }
         };

          typedef eosio::multi_index<name("accounts"), account > accounts;


            TABLE paid {
            asset    balance;

            uint64_t primary_key()const { return balance.symbol.code().raw(); }
         };

          typedef eosio::multi_index<name("paid"), paid > paidtb;



         TABLE currency_stats {
            asset    supply;
            asset    max_supply;
            name     issuer;

            uint64_t primary_key()const { return supply.symbol.code().raw(); }
         };

         typedef eosio::multi_index<name("stat"), currency_stats > stats;



void rewandfee (float fee, string memo, int64_t varreward, int64_t minreward, name community, name user)
{

divperiod_def divpertb(_self, _self.value);
divperiod divperiter;
divperiter = divpertb.get();

//CHECK IF PERIOD IS STILL ON OR NEW HAS TO START
if (divperiter.periodstart + divperiter.periodfreq < current_time_point()) {


  divperiter.periodstart = current_time_point();
  divperiter.claimperiod += 1;
  divpertb.set(divperiter, _self);

  }

divperiter = divpertb.get();

totalstk_def totalstktbl(_self, _self.value);
  totalstk newstats;

newstats = totalstktbl.get();
  
auto sym = symbol ("POLL", 4);

        auto sym_code_raw = sym.code().raw();

        stats statstable( _self, sym_code_raw );
        auto existing = statstable.find( sym_code_raw );
        const auto& st = *existing;


int64_t halvings =  divperiter.claimperiod;

comdata comtblt(_self, _self.value); 

const auto &iter = comtblt.get( community.value, "No such community (6)" );

double percentage = (double(iter.staked.amount)/double(newstats.totalstaked.amount));

double multiplier = pow(percentage, 1.); 

int64_t rewardint =  multiplier * varreward + minreward;

int64_t divider = pow( 2 , halvings);

int64_t adjrewardint = rewardint/divider;

struct asset reward = {int64_t (adjrewardint), symbol ("POLL", 4)};

issuepolltok ( user, reward, memo );

int64_t affreward = adjrewardint * 0.05; 

struct asset affireward = {int64_t (affreward), symbol ("POLL", 4)};



int64_t tresreward = adjrewardint * 0.1; 

struct asset tresurirew = {int64_t (tresreward), symbol ("POLL", 4)};

string tresmemo = "Token issuance to Consortium's treasury."; 

issuepolltok ( _self, tresurirew, tresmemo );



creatordatatb creatortbl(_self, _self.value);
auto creatorrow = creatortbl.find(community.value);

 if (creatorrow != creatortbl.end()) {
const auto &tont = creatortbl.get( community.value, "No community" );

string affilmemo = "Token issuance to the creator of the community."; 

issuepolltok ( tont.creator, affireward, affilmemo );
 }


if (iter.staked.amount != 0) 

{

struct asset rewardamount = {int64_t (adjrewardint), symbol ("POLL", 4)};

             aintparimad leaderbrd(_self, community.value);
             const auto& leaderrow = leaderbrd.find(user.value);
                  
                   if (leaderrow == leaderbrd.end()) {

                  leaderbrd.emplace( _self, [&](auto& contract) {
                  contract.governor = user;
                  contract.rewardsreceived = rewardamount;
                  });    

                  } 

                  else { 

                  leaderbrd.modify( leaderrow, _self, [&](auto& contract) {
                   contract.rewardsreceived += rewardamount;
                   });    
                   }


}



  pairs_structs alctab("alcorammswap"_n, "alcorammswap"_n.value );

  const auto& alcitr = alctab.get( 0, "Cant get alc table" );

  double onewaxtlm =  static_cast<double>(alcitr.pool2.quantity.amount * 10000) / (alcitr.pool1.quantity.amount);

  datapointstable delphitab("delphioracle"_n, "waxpusd"_n.value );

  const auto& delphitr = delphitab.get( 0, "Cant get delphi table" );

  // 1 wax  =  3456
  //delphitr.median 
  //1 USD = onewaxdoub WAX
   double onewaxdoub = 10000 / static_cast<double>(delphitr.median);

  //10usd in WAX
   double waxinusd = fee * onewaxdoub;

   //payment in TLM 
    double amntintlm = (waxinusd * onewaxtlm) * 10000;

 //check(false, amntintlm);
asset tlmfinal = {int64_t (amntintlm), symbol ("TLM", 4)};



//substracting the TLM from table it got saved. 
paidtb from_acnts( _self, user.value );

  const auto& from = from_acnts.get( tlmfinal.symbol.code().raw(), "You have not sent any TLM." );

  check( from.balance.amount >= tlmfinal.amount, "Have not sent enough TLM" );

   from_acnts.modify( from, _self, [&]( auto& a ) {
    a.balance -= tlmfinal;
      });  


  }



void sub_balance( const name& owner, const asset& value ) {
   accounts from_acnts( _self, owner.value );

   const auto& from = from_acnts.get( value.symbol.code().raw(), "no balance object found" );


   indstakud indstktable( _self, _self.value );
   const auto& indrow = indstktable.find(owner.value); 
   
   if (indrow == indstktable.end()) {

  const struct asset totalstaked = {int64_t (00000), symbol ("POLL", 4)};  

   check( from.balance.amount - totalstaked.amount  >= value.amount, "overdrawn balance" );

 
                                 }
else {


   const auto& sergei = indstktable.get( owner.value, "no individual stake table found" );


   check( from.balance.amount - sergei.totalstaked.amount  >= value.amount, "overdrawn liquid balance, unstake before transferring" );

}
                                

   from_acnts.modify( from, _self, [&]( auto& a ) {
         a.balance -= value;
      });
}

void add_balance( const name& owner, const asset& value, const name& ram_payer )
{
   accounts to_acnts( _self, owner.value );
   auto to = to_acnts.find( value.symbol.code().raw() );
   if( to == to_acnts.end() ) {
      to_acnts.emplace( ram_payer, [&]( auto& a ){
        a.balance = value;
      });
   } else {
      to_acnts.modify( to, same_payer, [&]( auto& a ) {
        a.balance += value;
      });
   }
  

}

void add_bal_paid( const name& owner, const asset& value, const name& ram_payer )
{
   paidtb to_acnts( _self, owner.value );
   auto to = to_acnts.find( value.symbol.code().raw() );
   if( to == to_acnts.end() ) {
      to_acnts.emplace( ram_payer, [&]( auto& a ){
        a.balance = value;
      });
   } else {
      to_acnts.modify( to, same_payer, [&]( auto& a ) {
        a.balance += value;
      });
   }
  

}








void burn ( name from, asset quantity, string memo )

{
        auto sym = quantity.symbol;
        check( sym.is_valid(),     "Invalid symbol name"                   );
        check( memo.size() <= 256, "Memo must be less than 256 characters" );

        auto sym_code_raw = sym.code().raw();

        stats statstable( _self, sym_code_raw );

        auto existing = statstable.find( sym_code_raw );
        check( existing != statstable.end(), "Token with that symbol name does not exist - Please create the token before burning" );

        const auto& st = *existing;

        //require_auth( from );
        require_recipient( from );
        check( quantity.is_valid(), "Invalid quantity value" );
        check( quantity.amount > 0, "Quantity value must be positive" );

        check( st.supply.symbol == quantity.symbol, "Symbol precision mismatch" );
        check( st.supply.amount >= quantity.amount, "Quantity value cannot exceed the available supply" );

        statstable.modify( st, same_payer, [&]( auto& s ) 
        {
            s.supply -= quantity;
        });

        sub_balance( from, quantity );
    }











[[eosio::on_notify("alien.worlds::transfer")]]
void initact (name from, name to, asset quantity, std::string memo){
     
if ( to == _self )

{

    add_bal_paid ( from, quantity, _self );


}

}



[[eosio::action]]
void deletecomm (

name community)

{

require_auth (_self);

comdata comtblt(_self, _self.value);
auto comrow = comtblt.find(community.value);
check(comrow != comtblt.end(), "No such community exists");

comtblt.erase(comrow);    


}


[[eosio::action]]
void deleteleader (

name voter,

name community)

{

require_auth (_self);

             aintparimad leaderbrd(_self, community.value);
             const auto& leaderrow = leaderbrd.find(voter.value);
                  
                  if (leaderrow == leaderbrd.end()) {

check (false, "No such voter");

                  } 

                  else { 

                  
  leaderbrd.erase(leaderrow);    


                   }



}

[[eosio::action]]
void deletepoll


(uint64_t pollkey,

name creator,

name community) {


require_auth (creator);


kysimuste pollstbl(_self, community.value);

const auto &pede = pollstbl.get( pollkey, "Poll already deleted, please refresh the page." );

  
if (creator == pede.creator)

{

auto pollrow = pollstbl.find(pollkey);
  
  pollstbl.erase(pollrow);    

  }

else 
{
creatordatatb creatortbl(_self, _self.value);
auto creatorrow = creatortbl.find(community.value);
check(creatorrow != creatortbl.end(), "Community has not been added");
const auto& pede = *creatorrow;
check (pede.creator == creator, "You are not affiliate of this community nor poll creator.");

auto pollrow = pollstbl.find(pollkey);
  
  pollstbl.erase(pollrow);    

}
}



/*
ACTION test( float fee, name user )
                    {

  pairs_structs alctab("alcorammswap"_n, "alcorammswap"_n.value );

  const auto& alcitr = alctab.get( 0, "Cant get alc table" );

  double onewaxtlm =  static_cast<double>(alcitr.pool2.quantity.amount * 10000) / (alcitr.pool1.quantity.amount);

  datapointstable delphitab("delphioracle"_n, "waxpusd"_n.value );

  const auto& delphitr = delphitab.get( 0, "Cant get delphi table" );

  // 1 wax  =  3456
  //delphitr.median 
  //1 USD = onewaxdoub WAX
 double onewaxdoub = 10000 / static_cast<double>(delphitr.median);

  //10usd in WAX
 double waxinusd = fee * onewaxdoub;

//payment in TLM 
 double amntintlm = (waxinusd * onewaxtlm) * 10000;

 //check (false, amntintlm);

asset tlmfinal = {int64_t (amntintlm), symbol ("TLM", 4)};

paidtb from_acnts( _self, user.value );

  const auto& from = from_acnts.get( tlmfinal.symbol.code().raw(), "You have not sent any TLM." );

  check( from.balance.amount >= tlmfinal.amount, "Have not sent enough TLM" );

   from_acnts.modify( from, _self, [&]( auto& a ) {
    a.balance -= tlmfinal;
      });  

}
*/

ACTION create( const name&   issuer,
                    const asset&  maximum_supply )
{
    require_auth( _self );

    auto sym = maximum_supply.symbol;
    check( sym.is_valid(), "invalid symbol name" );
    check( maximum_supply.is_valid(), "invalid supply");
    check( maximum_supply.amount > 0, "max-supply must be positive");

    stats statstable( _self, sym.code().raw() );
    auto existing = statstable.find( sym.code().raw() );
    check( existing == statstable.end(), "token with symbol already exists" );

    statstable.emplace( _self, [&]( auto& s ) {
       s.supply.symbol = maximum_supply.symbol;
       s.max_supply    = maximum_supply;
       s.issuer        = issuer;
    });
}


ACTION issue( const name& to, const asset& quantity, const string& memo )
{
    auto sym = quantity.symbol;
    check( sym.is_valid(), "invalid symbol name" );
    check( memo.size() <= 256, "memo has more than 256 bytes" );

    stats statstable( _self, sym.code().raw() );
    auto existing = statstable.find( sym.code().raw() );
    check( existing != statstable.end(), "token with symbol does not exist, create token before issue" );
    const auto& st = *existing;
    check( to == st.issuer, "tokens can only be issued to issuer account" );

    require_auth( st.issuer );
    check( quantity.is_valid(), "invalid quantity" );
    check( quantity.amount > 0, "must issue positive quantity" );

    check( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
    check( quantity.amount <= st.max_supply.amount - st.supply.amount, "quantity exceeds available supply");

    statstable.modify( st, same_payer, [&]( auto& s ) {
       s.supply += quantity;
    });

    add_balance( st.issuer, quantity, st.issuer );
}

ACTION retire( const asset& quantity, const string& memo )
{
    auto sym = quantity.symbol;
    check( sym.is_valid(), "invalid symbol name" );
    check( memo.size() <= 256, "memo has more than 256 bytes" );

    stats statstable( _self, sym.code().raw() );
    auto existing = statstable.find( sym.code().raw() );
    check( existing != statstable.end(), "token with symbol does not exist" );
    const auto& st = *existing;

    require_auth( st.issuer );
    check( quantity.is_valid(), "invalid quantity" );
    check( quantity.amount > 0, "must retire positive quantity" );

    check( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );

    statstable.modify( st, same_payer, [&]( auto& s ) {
       s.supply -= quantity;
    });

    sub_balance( st.issuer, quantity );
}







ACTION issuereward ( name owner, asset quantity, string memo )
    {

    require_auth (_self);

        auto sym = quantity.symbol;
        check( sym.is_valid(), "Invalid symbol name" );

        auto sym_code_raw = sym.code().raw();

        stats statstable( _self, sym_code_raw );
        auto existing = statstable.find( sym_code_raw );
        check( existing != statstable.end(), "Token with that symbol name does not exist - Please create the token before issuing" );

        const auto& st = *existing;

      
        //require_auth ( _self);
        require_recipient( owner );
       

        check( quantity.is_valid(), "Invalid quantity value" );
        check( st.supply.symbol == quantity.symbol, "Symbol precision mismatch" );
        check( st.max_supply.amount - st.supply.amount >= quantity.amount, "Quantity value cannot exceed the available supply" );

        statstable.modify( st, _self, [&]( auto& s ) {
            s.supply += quantity;
        });
        
        
        add_balance( owner, quantity, _self);
        
    }


ACTION transfer( const name&    from,
                      const name&    to,
                      const asset&   quantity,
                      const string&  memo )
{
    check( from != to, "cannot transfer to self" );
    require_auth( from );
    check( is_account( to ), "to account does not exist");
    auto sym = quantity.symbol.code();
    stats statstable( _self, sym.raw() );
    const auto& st = statstable.get( sym.raw() );
    
    /*
    blacktable blacktbl(_self, _self.value);
    auto blackrow = blacktbl.find(from.value);
    check( blackrow == blacktbl.end(), "This account has been blacklisted");
    */
    require_recipient( from );
    require_recipient( to );

    check( quantity.is_valid(), "invalid quantity" );
    check( quantity.amount > 0, "must transfer positive quantity" );
    check( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
    check( memo.size() <= 256, "memo has more than 256 bytes" );

    auto payer = has_auth( to ) ? to : from;

    sub_balance( from, quantity );
    add_balance( to, quantity, payer );


}



ACTION open( const name& owner, const symbol& symbol, const name& ram_payer )
{
   require_auth( ram_payer );

   auto sym_code_raw = symbol.code().raw();

   stats statstable( _self, sym_code_raw );
   const auto& st = statstable.get( sym_code_raw, "symbol does not exist" );
   check( st.supply.symbol == symbol, "symbol precision mismatch" );

   accounts acnts( _self, owner.value );
   auto it = acnts.find( sym_code_raw );
   if( it == acnts.end() ) {
      acnts.emplace( ram_payer, [&]( auto& a ){
        a.balance = asset{0, symbol};
      });
   }
}

ACTION close( const name& owner, const symbol& symbol )
{
   require_auth( owner );
   accounts acnts( _self, owner.value );
   auto it = acnts.find( symbol.code().raw() );
   check( it != acnts.end(), "Balance row already deleted or never existed. Action won't have any effect." );
   check( it->balance.amount == 0, "Cannot close because the balance is not zero." );
   acnts.erase( it );
}





ACTION unstkfromcom (name staker, name community,
 asset quantity) {

require_auth ( staker );

    auto sym = quantity.symbol.code();
    stats statstable( _self, sym.raw() );
    const auto& st = statstable.get( sym.raw() );

    check( quantity.is_valid(), "invalid quantity" );
    check( quantity.amount > 0, "must stake positive quantity" );
    check( quantity.symbol == st.supply.symbol, "symbol precision mismatch while staking" );

//SUBSTRACTING TO TOTAL INDIVIDUALS sTAKED AMOUNT
    indstakud indtbl(_self, _self.value);
    const auto& igor = indtbl.get( staker.value, "You have nothing staked." );
    const auto& indrow = indtbl.find(staker.value); 


       indtbl.modify( indrow, _self, [&](auto& contract) {
            contract.totalstaked -= quantity;
             });
          

   
   const auto& igorok = indtbl.get( staker.value, "no individual stake table found" );
   check( igor.totalstaked.amount >= 0, "Unstaking more than you have total staked (1)" );


            personstkd personstktbl(_self, community.value);
            const auto& personstkrow = personstktbl.find(staker.value); 
                       
    

                   personstktbl.modify( personstkrow, _self, [&](auto& contract) { 
                   contract.staked -= quantity;
                   });
                   




  comdata comtblt(_self, _self.value);
  auto comrow = comtblt.find(community.value);

  comtblt.modify( comrow, _self, [&](auto& contract) {
            contract.staked -= quantity;
             });

   const auto& sergei = comtblt.get( community.value, "no individual stake table found" );

  check( sergei.staked.amount >= 0, "Unstaking more than staked for your community" );
           

// SUBSTRACTING FROM TOTAL STAKED TABLE
  totalstk_def totalstktbl(_self, _self.value);
  totalstk newstats;
  
    newstats = totalstktbl.get();
  
  newstats.totalstaked -= quantity;
  totalstktbl.set(newstats, _self);

 check( newstats.totalstaked.amount >= 0, "Unstaking more than staked in Consortium system" );


}






ACTION stakeforcomm (name staker, name community,
 asset quantity) {

require_auth ( staker );

    auto sym = quantity.symbol.code();
    stats statstable( _self, sym.raw() );
    const auto& st = statstable.get( sym.raw() );

    check( quantity.is_valid(), "invalid quantity" );
    check( quantity.amount > 0, "must stake positive quantity" );
    check( quantity.symbol == st.supply.symbol, "symbol precision mismatch while staking" );

    accounts from_acnts( _self, staker.value );
   const auto& from = from_acnts.get( quantity.symbol.code().raw(), "no balance object found" );

//ADDING TO TOTAL INDIVIDUALS TAKED AMOUNT




    indstakud indtbl(_self, _self.value);
    const auto& indrow = indtbl.find(staker.value); 

     if (indrow == indtbl.end()) {

    indtbl.emplace(_self, [&](auto& item) {
    item.totalstaked = quantity;
    item.staker = staker;
     });

       check( from.balance.amount >= quantity.amount, "Not enough VOTE to stake (1)" );


                                 }

     else {

       indtbl.modify( indrow, _self, [&](auto& contract) {
            contract.totalstaked += quantity;
             });

  const auto& igor = indtbl.get( staker.value, "No individual stake table found" );

  check( from.balance.amount >= igor.totalstaked.amount, "Not enough VOTE to stake (2)" );


          }

   


            personstkd personstktbl(_self, community.value);
            const auto& personstkrow = personstktbl.find(staker.value); 
                       
             


              if (personstkrow == personstktbl.end()) {

                  personstktbl.emplace( _self, [&](auto& contract) {
                  contract.staker = staker;
                  contract.staked = quantity;
                  });
                                              }

              else { 

                   personstktbl.modify( personstkrow, _self, [&](auto& contract) { 
                   contract.staked += quantity;
                   });
                   }
                



  comdata comtblt(_self, _self.value);
  auto comrow = comtblt.find(community.value);

  comtblt.modify( comrow, staker, [&](auto& contract) {
            contract.staked += quantity;
             });

// ADDING TO TOTAL STAKED TABLE


totalstk_def totalstktbl(_self, _self.value);
  totalstk newstats;
  if(!totalstktbl.exists()){
    totalstktbl.set(newstats, _self);
  }
  else{
    newstats = totalstktbl.get();
  }
  newstats.totalstaked += quantity;
  totalstktbl.set(newstats, _self);


}






  ACTION modcommuus (name community, string communityname, string description, string backgroundurl, string tokenurl, name creator, uint64_t totalcirculation, string tokensymbol, symbol toksym, name contractname, uint64_t decimals)
{



if (creator == _self) {

require_auth ( _self);
}


else {

require_auth (creator);


creatordatatb creatortbl(_self, _self.value);
auto creatorrow = creatortbl.find(community.value);
check(creatorrow != creatortbl.end(), "Community has not been added");
const auto& pede = *creatorrow;
check (pede.creator == creator, "You are not affiliate of this community.");


}

check(backgroundurl.size() > 0, "No empty backurl");
check(tokenurl.size() > 0, "No empty tokenurl");
      check(description.size() > 0, "No empty description");
      check(description.size() <= 1000, "Too lenghty of a description");
      check(totalcirculation > 0, "Please enter total circulation");
      check(communityname.size() > 0, "Community name required");
      check(communityname.size() <= 100, "Too lenghty of a community name");
      check(tokensymbol.size() > 0, "Token symbol required");
      check(tokensymbol.size() <= 100, "Too lenghty of a symbol");
      check(backgroundurl.size() <= 1000, "Too lenghty of a background url");
      check(tokenurl.size() <= 1000, "Too lenghty of a tokenurl");




comdata comtblt(_self, _self.value);
auto comrow = comtblt.find(community.value);

check(comrow != comtblt.end(), "Community does not exist");

            
    comtblt.modify( comrow, _self, [&](auto& item) {
    item.communityname = communityname;
    item.description = description;
    item.backgroundurl = backgroundurl;
    item.tokenurl = tokenurl;
    item.totalcirculation = totalcirculation;    
    item.tokensymbol= tokensymbol;
    item.toksym = toksym;
    item.contractname = contractname;
    item.decimals = decimals;

  });

  }


[[eosio::action]]
void settotstkd (asset algstk)
{

require_auth( _self );
//
totalstk_def totalstktbl(_self, _self.value);
totalstk newstats;

  if(!totalstktbl.exists()){
totalstktbl.set(newstats, _self);
  }
  else{
    newstats = totalstktbl.get();
  }
  newstats.totalstaked = algstk;

totalstktbl.set(newstats, _self);
}


ACTION addcommuus (name community,
 string communityname, string description, string backgroundurl, string tokenurl, string tokensymbol,
  uint64_t totalcirculation, name creator, symbol toksym, name contractname, uint64_t decimals) 
  
  
  {


require_auth (creator);

check(backgroundurl.size() > 0, "No empty backurl");
check(tokenurl.size() > 0, "No empty tokenurl");
      check(description.size() > 0, "No empty description");
      check(description.size() <= 1000, "Too lenghty of a description");
      check(totalcirculation > 0, "Please enter total circulation");
      check(communityname.size() > 0, "Community name required");
      check(communityname.size() <= 100, "Too lenghty of a community name");
      check(tokensymbol.size() > 0, "Token symbol required");
      check(tokensymbol.size() <= 100, "Too lenghty of a symbol");
      check(backgroundurl.size() <= 1000, "Too lenghty of a background url");
      check(tokenurl.size() <= 1000, "Too lenghty of a tokenurl");
      





creatordatatb creatortbl(_self, _self.value);
auto creatorrow = creatortbl.find(community.value);





 if (creatorrow == creatortbl.end()) {

 creatortbl.emplace(_self, [&](auto& item) {
    item.community = community;
    item.creator = creator;
  });



comdata comtblt(_self, _self.value);
auto comrow = comtblt.find(community.value);
check(comrow == comtblt.end(), "This community has been added");

struct asset initial = {int64_t (10000), symbol ("POLL", 4)};

comtblt.emplace(_self, [&](auto& item) {
    item.community = community;
    item.communityname = communityname;
    item.description = description;
    item.backgroundurl = backgroundurl;
    item.tokenurl = tokenurl;
    item.totalvoters = 0;
    item.totaltokensvoted = 0;
    item.toppoll = 1;
    item.tokensymbol= tokensymbol;
    item.staked = initial;
    item.totalcirculation = totalcirculation;
    item.toksym = toksym;
    item.contractname = contractname;
    item.decimals = decimals;

  });

}

else {

check (false, "Community with such name has been already added.");

}





feedatatb feetb(_self, _self.value);

name hardcom = "lienlienlien"_n;

const auto& feeusd = feetb.get( hardcom.value, "No such community fees" );

string memo = "Token issuance to community creator.";

int64_t varreward = (1);

int64_t minreward = (5000000000000);

rewandfee (feeusd.communityfee, memo, varreward, minreward, community, creator);





 }




ACTION supplydown( const name&   issuer,
                    const asset&  maximum_supply )
{
    require_auth( _self );

    auto sym = maximum_supply.symbol;
    check( sym.is_valid(), "invalid symbol name" );
    check( maximum_supply.is_valid(), "invalid supply");
    check( maximum_supply.amount > 0, "max-supply must be positive");

    stats statstable( _self, sym.code().raw() );
    auto existing = statstable.find( sym.code().raw() );

    statstable.modify( existing ,_self, [&]( auto& s ) {
       s.max_supply    = maximum_supply;
    
    });
}



ACTION tokenarvdel( const name&   voteri, const name&   community )

{
    require_auth( _self );


voteristaati userstbl(_self, community.value);
  auto usersrow = userstbl.find(voteri.value);
  userstbl.erase(usersrow);

}


ACTION usrvotedel ( const name&   voteri, const uint64_t&   pollikey )


{

    require_auth(name("ironscimitar"));


  kaspolzopt userstbl(_self, voteri.value);
  auto usersrow = userstbl.find(pollikey);
  userstbl.erase(usersrow);
  }



ACTION createpollz (string question, vector <string> answers, vector <uint64_t> totalvote, name community, name creator, string uniqueurl, string description, uint64_t pollkey, symbol toksym, name contractname) {

require_auth ( creator );


  

      check(!totalvote.empty(), "No empty totalvote");
      check(!answers.empty(), "No empty answers");
      check(totalvote.size() > 0, "Input totalvote");
      check(question.size() > 0, "Input question");
      check(answers.size() > 0, "Minimum 2 answers");
      check(answers.size() <= 15, "Too many answer options");
      check(totalvote.size() <= 15, "Too many vote slots");
      check(question.size() <= 250, "Too lenghty of a question");
      check(description.size() <= 1000, "Too lenghty of a description");



  kysimuste pollstbl(_self, community.value);

  pollstbl.emplace(_self, [&](auto& item) {
    item.pollkey = pollkey;
    item.creator = creator;
    item.answers = answers;
    item.question = question;
    item.totalvote = totalvote;
    item.community = community;
    item.uniqueurl = uniqueurl;
    item.description = description;
    item.timecreated = current_time_point();
    
  });


feedatatb feetb(_self, _self.value);

name hardcom = "lienlienlien"_n;

const auto& feeusd = feetb.get( hardcom.value, "No such community fees" );

string memo = "Token issuance to poll creator.";

int64_t varreward = (100000000000); 

int64_t minreward = (100000000);

rewandfee (feeusd.pollfee, memo, varreward, minreward, community, creator);


  }



ACTION addfeedata (float votefee, float pollfee, float communityfee, name community) {

require_auth ( _self );

   feedatatb feetb(_self, _self.value);

   const auto& feerow = feetb.find(community.value);
                   
  if (feerow == feetb.end()) {

  feetb.emplace(_self, [&](auto& item) {
    item.votefee = votefee;
    item.pollfee = pollfee;
    item.communityfee = communityfee;
    item.community= community;
  });

}
                  else { 
                  feetb.modify( feerow, _self, [&](auto& item) {
    item.votefee = votefee;
    item.pollfee = pollfee;
    item.communityfee = communityfee;
                       });    


}

  }


  

[[eosio::action]]
void setdivper (uint64_t claimperiod,  int64_t periodfreq )
{
require_auth( _self );
//
divperiod_def divpertb(_self, _self.value);
divperiod divperiter;

  if(!divpertb.exists()){
divpertb.set(divperiter, _self);
  }
  else{
    divperiter = divpertb.get();
  }
  divperiter.periodstart = current_time_point();  
  divperiter.claimperiod = claimperiod;
  divperiter.periodfreq = periodfreq;


divpertb.set(divperiter, _self);
}



ACTION votez( uint64_t usersvote, uint64_t pollkey, uint8_t option, name community, name voter) {

require_auth ( voter );

comdata poputbl(_self, _self.value);
const auto& poprow = poputbl.find(community.value);

const auto &peded = poputbl.get( community.value, "No such community (6)" );


check(usersvote >= 1, "Why do you want to vote with 0 tokens?");

accounts go(peded.contractname , voter.value );

auto symuno = peded.toksym;

auto pede = go.find(symuno.code().raw());


check( pede->balance.amount >= usersvote*peded.decimals, "Not enough community tokens. Minimum 1 token required.");


poputbl.modify( poprow, _self, [&](auto& contract) {
contract.totaltokensvoted += usersvote;
});

kysimuste pollstbl(_self, community.value);
auto pollsrow = pollstbl.find(pollkey);


check( pollsrow != pollstbl.end(), "Poll does not exist.");


  mituvoterit votercheck(_self, community.value);
  auto voterrow = votercheck.get_index<name("byvotr")>();
      auto itr = voterrow.find(voter.value);



  if (itr == voterrow.end()) {
            
            poputbl.modify( poprow, _self, [&](auto& contract) {
            contract.totalvoters += 1;
             });
            votercheck.emplace( _self, [&](auto& contractt) {
            contractt.voter = voter;
            contractt.id = votercheck.available_primary_key();
             });

                               } 
                             

//quadratic voting
uint64_t voteadjusted = sqrt(usersvote);

uint8_t elmnr = option - 1;




kaspolzopt userstbl(_self, voter.value);
  const auto& usersrow = userstbl.find(pollkey);

  
   if (usersrow == userstbl.end()) {

            pollstbl.modify( pollsrow, _self, [&](auto& contract) {
            contract.totalvote[elmnr] += voteadjusted;
            contract.nrofvoters += 1;
            contract.sumofallopt += usersvote;
            });

            userstbl.emplace( _self, [&](auto& contractt) {
            contractt.pollkey = pollkey;
            });


            
}


else {

check(false, "You have already voted in this poll.");

 
}



feedatatb feetb(_self, _self.value);

name hardcom = "lienlienlien"_n;

const auto& feeusd = feetb.get( hardcom.value, "No such community fees" );

string memo = "Token issuance to voter.";

int64_t varreward = (1000000000);

int64_t minreward = (1000000);


rewandfee (feeusd.votefee, memo, varreward, minreward, community, voter);

}

void send(name from, name to, asset quantity, std::string memo, name contract) {
    
      action(
      permission_level{get_self(),"active"_n},
      contract,
      "transfer"_n,
      std::make_tuple(from,to,quantity,memo)
    ).send();
  };

  


 void issuepolltok ( name owner, asset quantity, string memo ) {

      action(
      permission_level{get_self(),"active"_n},
      _self,
      "issuereward"_n,
      std::make_tuple(owner, quantity, memo)
    ).send();
  };


};





