import Header from "../components/header"

import DownloadSection from "../components/download-section"
import Introduction from "../components/introduction"
import Features from "../components/features"
import Footer from "../components/footer"
import Examples from "../components/examples"

import HomeLayout from "../layouts/home-layout"

function HomePage() {
  return (
    <>
      <HomeLayout>
        <Header />
        <Introduction />
        <DownloadSection />
        <Features />
        <Examples />
      </HomeLayout>
      <Footer />
    </>
  )
}

export default HomePage;
